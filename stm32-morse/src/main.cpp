#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "led.h"
#include "timer.h"

namespace {
constexpr timer::ticks_t DIT_DURATION = timer::FREQUENCY_HZ;
} // namespace

// constexpr timer::ticks_t DIT_DURATION = timer::FREQUENCY_HZ;
// Assuming STM32F4-Discovery board, PD12-PD15, active high.

#define BLINK_PORT_NUMBER (3)
#define BLINK_PIN_NUMBER_GREEN (12)
#define BLINK_PIN_NUMBER_ORANGE (13)
#define BLINK_PIN_NUMBER_RED (14)
#define BLINK_PIN_NUMBER_BLUE (15)
#define BLINK_ACTIVE_LOW (false)

led morseLed = {BLINK_PORT_NUMBER, BLINK_PIN_NUMBER_GREEN, BLINK_ACTIVE_LOW};

// International Morse Code
// The duration of a dot(dit) is one unit
// dash(dah) = 3 units
// space between parts of same letter = one unit
// space between letters = 3 units
// space between words = 7 units

// 0 is dot(dit), 1 is dash(dah)
// more readable in my opinion
#define DIT '0'
#define DAH '1'
std::map<const char, std::string> internationalMorseCodeMap = {
    {'A', "01"},    {'B', "1000"},  {'C', "1010"},  {'D', "100"},
    {'E', "0"},     {'F', "0010"},  {'G', "110"},   {'H', "0000"},
    {'I', "00"},    {'J', "0111"},  {'K', "101"},   {'L', "0100"},
    {'M', "11"},    {'N', "10"},    {'O', "111"},   {'P', "0110"},
    {'Q', "1101"},  {'R', "010"},   {'S', "000"},   {'T', "1"},
    {'U', "001"},   {'V', "0001"},  {'W', "011"},   {'X', "1001"},
    {'Y', "1011"},  {'Z', "1100"},  {'1', "01111"}, {'2', "00111"},
    {'3', "00011"}, {'4', "00001"}, {'5', "00000"}, {'6', "10000"},
    {'7', "11000"}, {'8', "11100"}, {'9', "11110"}, {'0', "11111"},
};

int main() {
  timer timer;
  timer.start();

  morseLed.power_up();

  std::string msg = "SOS";

  // TODO: validate characters
  for (char c : msg) {
    for (char morseSymbol : internationalMorseCodeMap[c]) {
      morseLed.turn_on();

      if (morseSymbol == DIT) {
        timer.sleep(DIT_DURATION);
      } else {
        timer.sleep(DIT_DURATION * 3);
      }

      morseLed.turn_off();

      timer.sleep(DIT_DURATION);
    }

    if (c == ' ' || c == '\n') {
      timer.sleep(DIT_DURATION * 7);
    } else {
      timer.sleep(DIT_DURATION * 3);
    }
  }

  return 0;
}
