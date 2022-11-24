#define DATA_PIN 9
#define DATA_LEVEL LOW

const int TU = 50;
#define LETTER_SEP 3
#define WORD_SEP 7

const String data_encoded[] = { ".-", "-...", "-..-", "--.." };
const char data_letters[] = { 'A', 'B', 'X', 'Z' };
constexpr int size_arr_data = sizeof(data_letters) / sizeof(char);

void setup()
{
  Serial.begin(9600);
  Serial.print("> ");
  
  pinMode(DATA_PIN, OUTPUT);
  digitalWrite(DATA_PIN, !DATA_LEVEL);
}

bool wait_input = true;
char letter = ' ';

void loop()
{
  if (Serial.available() >= 1)
  {
    if (wait_input)
    {
        wait_input = false;
        Serial.print("Send: ");
    }
    letter = Serial.read();

    if (letter == ' ')
    {
        send_sep_word();
    }
    else
    {
        for (int it = 0; it < size_arr_data; ++it)
        {
            if (letter == data_letters[it])
            {
            Serial.print(data_letters[it]);
                send_data(data_encoded[it]);
                break;
            }
        }
    }
  }
  else if (!wait_input)
  {
    send_sep_word();
    Serial.print("\n> ");
    wait_input = true;
  }
}

void send_data(const String& message)
{
  int len = message.length();

  for (int it = 0; it < len; ++it)
  {
    if (message[it] == '.')
    {
      digitalWrite(DATA_PIN, DATA_LEVEL);
      delay(TU);
    }
    else
    {
      digitalWrite(DATA_PIN, DATA_LEVEL);
      delay(3*TU);
    }
    digitalWrite(DATA_PIN, !DATA_LEVEL);
    delay(TU);  // задержка между данными одного символа
  }
  delay((LETTER_SEP - 1) * TU);  // задержка между буквами (1TU уже выждан в цикле)
}

void send_sep_word()
{
        digitalWrite(DATA_PIN, !DATA_LEVEL);
        delay((WORD_SEP - LETTER_SEP) * TU);   // задержка между словами (LETTER_SEP * TU уже выждан в цикле)
        Serial.print(" ");
}
