#define DATA_PIN 2
#define DATA_LEVEL LOW

const int TU = 50;
#define LETTER_SEP 3
#define WORD_SEP 7
#define IDLE_TIME 10

long start = 0;

const String data_encoded[] = { ".-", "-...", "-..-", "--.." };
const char data_letters[] = { 'A', 'B', 'X', 'Z' };
constexpr int size_arr_data = sizeof(data_letters) / sizeof(char);

bool check_letter = false;
bool is_idle = true;

int current = 0;
bool new_data = true;
bool end_word = false;

int timings[20];
bool levels[20];

void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
  attachInterrupt(0, process_timing, CHANGE ); // 0 -> digitalPin 2
}

void loop()
{
  process_idle();

  if (check_letter)
  {
    recieve_letter();  
    check_letter = false;
  }
}

void recieve_letter()
{
    if (timings[current - 1] >= LETTER_SEP && levels[current - 1] != DATA_LEVEL)
    {
        String letter ="";

        for (int i = 0; i < current; ++i)
        {
            if (levels[i] == DATA_LEVEL)
            {
                if (timings[i] == 1)
                {
                    letter += '.';
                }
                else if (timings[i] == 3)
                {
                    letter += '-';
                }
            }
        }

        if (new_data)
        {
          Serial.print("\nGet: ");
          new_data = false;
        }

        if (timings[current - 1] >= IDLE_TIME)
        {
            new_data = true;
        }
        else if (timings[current - 1] >= WORD_SEP)
        {
            end_word = true;
        }

        current = 0;

        for (int it = 0; it < size_arr_data; ++it)
        {
            if (data_encoded[it] == letter)
            {
                Serial.print(data_letters[it]);
            }
        }

        if (end_word)
        {
            Serial.print(" ");
            end_word = false;
        }
    }
}

void process_timing()
{
  if (!is_idle)
  {
    timings[current] = round((millis() - start) /TU);  // ?
    levels[current] = !digitalRead(DATA_PIN);
    
    ++current;

    check_letter = true;
  }

  start = millis();
  is_idle = false;
}

void process_idle()
{
  if (!is_idle && round((millis() - start) / TU) >= IDLE_TIME)
  {
    timings[current] = IDLE_TIME;
    levels[current] = !DATA_LEVEL;

    ++current;

    start = millis();
    is_idle = true;

    check_letter = true;
  }
}