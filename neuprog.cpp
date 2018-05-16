#include <stdint.h>
#include <stdio.h>

#include <pigpio.h>

static const int CS = 5;
static const int CK = 21;
static const int mMOSI = 20;
static const int mMISO = 19;

static const int LOW = 0;
static const int HIGH = 1;
#define PIGPIOFACTOR 100

void setup()
{
  gpioInitialise();
  gpioSetMode(CS, PI_OUTPUT);
  gpioSetMode(CK, PI_OUTPUT);
  gpioSetMode(mMOSI, PI_OUTPUT);
  gpioSetMode(mMISO, PI_INPUT);
  printf("Setup done...\n");
}

// Write out the N low order bits of data as MSB first
// and return the collected data.
uint32_t write_N_MSB_first(uint32_t data, int n)
{
  uint32_t ret = 0;

  // First shift everything to the 16th bit
  data <<= (32 - n);
  gpioWrite(mMOSI, LOW); // Make the scope trace look nice
  gpioDelay(1 * PIGPIOFACTOR);

  for (int i = 0; i < n; i++)
  {
    gpioWrite(mMOSI, (bool)(data & (uint32_t(1) << 31)));
    gpioDelay(1 * PIGPIOFACTOR);
    gpioWrite(CK, HIGH);
    gpioDelay(1 * PIGPIOFACTOR);
    gpioWrite(CK, LOW);
    gpioDelay(1 * PIGPIOFACTOR);
    int r = gpioRead(mMISO);
    ret <<= 1;
    ret |= r;
    data <<= 1;
  }
  gpioWrite(mMOSI, LOW); // Make the scope trace look nice
  return ret;
}

void ewen()
{
  gpioWrite(CS, 1);
  write_N_MSB_first(0x260, 10);
  gpioWrite(CS, 0);
  gpioDelay(1 * PIGPIOFACTOR);
}

void ewds()
{
  gpioWrite(CS, 1);
  write_N_MSB_first(0x200, 10);
  gpioWrite(CS, 0);
  gpioDelay(1 * PIGPIOFACTOR);
}

uint8_t read(uint8_t addr)
{
  gpioWrite(CS, 1);
  write_N_MSB_first(0x300 | (addr & 0x7f), 10);
  int v = write_N_MSB_first(0, 8);
  gpioWrite(CS, 0);
  gpioDelay(1 * PIGPIOFACTOR);
  return v;
}

void write(uint8_t addr, uint8_t val)
{
  gpioWrite(CS, 1);
  write_N_MSB_first(0x280 | (addr & 0x7f), 10);
  write_N_MSB_first(val, 8);
  gpioWrite(CS, 0);
  gpioDelay(15 * PIGPIOFACTOR);
}

void dump_eeprom()
{
  printf("dump begin\n");

  unsigned char data[128];
  for (int i = 0; i < 128; i++)
    data[i] = read(i);

  for (int i = 0; i < 128; i++)
  {
    char buf[16];
    if (i % 16 == 0)
    {
      sprintf(buf, "\n0x%02x:", i);
      fputs(buf, stdout);
    }

    sprintf(buf, " %02x", data[i]);
    fputs(buf, stdout);
  }
  puts("");
}

void read_count()
{
  printf("read count\n");

  int v1 = read(0x55);
  int v2 = read(0x50);
  printf("0x55, 0x50 :=  0x%x 0x%x\n", v1, v2);
}

void write_count(int new_count)
{
  printf("write count\n");

  ewen();
  write(0x55, new_count);
  write(0x50, new_count - 1);
  ewds();

  int v3 = read(0x55);
  int v4 = read(0x50);

  printf("new 0x55, 0x50 :=  0x%x 0x%x\n", v3, v4);
}

void all(int state)
{
  gpioWrite(CS, state);
  gpioWrite(CK, state);
  gpioWrite(mMOSI, state);
}

int main()
{
  printf("Simple bitbang SPI EEPROM tool for AT93C64D\n");
  do
  {
    setup();
    dump_eeprom();
    read_count();
    write_count(0x9a);
    dump_eeprom();
  } while (0);

  all(LOW);
  gpioTerminate();
  printf("the end\n");

  return 0;
}