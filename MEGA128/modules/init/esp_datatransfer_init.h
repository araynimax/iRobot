#include <twi.h>
#define TWI_BUFFER_SIZE 32
int esp_datatransfer_enabled=0;

// 7bit slave I2C address
#define TWI_SLAVE_ADDR 0x50

// flag that signals that the TWI slave reception was OK
bit esp_datatransfer_received_ok=false;

struct TSensor{

};

struct TData{
    int action;
    struct TSensor sensor;
};

union TBuffer{
    char bytes[32];
    struct TData data;
};

union TBuffer rx_buffer;  // slave receive buffer
union TBuffer tx_buffer;  // slave transmission buffer

flash char * flash status_msg[8]=
{
"OK",
"Buffer overflow",
"Arbitration lost",
"Bus error",
"NACK received",
"Bus timeout",
"Fail",
"Unknown error"
};

bool slave_rx_handler(bool rx_complete)
{
  if (twi_result==TWI_RES_OK)
    received_ok=true; // signal that data was received without errors
  else
  {
    // TWI receive error, display the twi_result value
    lcd_clear();
    lcd_putsf("Receive error:\n");
    lcd_putsf(status_msg[twi_result]);
    received_ok=false; // signal that data was received with errors
    return false; // stop reception
  }

  if (rx_complete)
    // the TWI master has finished transmitting data
    return false; // no more bytes to receive

  // signal to the TWI master that the TWI slave is ready to accept more data
  // as long as there is space in the receive buffer
  return (twi_rx_index<TWI_BUFFER_SIZE);
}

unsigned char slave_tx_handler(bool tx_complete)
{
  if (tx_complete==false)
  {


    // number of bytes to transmit from the TWI slave to the TWI master
    return TWI_BUFFER_SIZE;
  }

  // transmission from slave to master has already started,
  // no more bytes to send in this transaction
  if (received_ok)
  {
    lcd_clear();
  }
  return 0;
}

void esp_datatransfer_init(){
    twi_slave_init(false,TWI_SLAVE_ADDR,rx_buffer.bytes,TWI_BUFFER_SIZE,tx_buffer.bytes,slave_rx_handler,slave_tx_handler);
}
