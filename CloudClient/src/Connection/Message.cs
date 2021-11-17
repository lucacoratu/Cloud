using System;
using System.Collections.Generic;
using System.Text;

namespace CloudClient.src.Connection
{
    //Header of the message
    struct MessageHeader
    {
        public byte action;
        public byte errorNo;
        public int dataLength;
    }

    //The full message containing the header and the message data
    class Message
    {
        private MessageHeader header;
        private Byte[] data;


        //Defualt constructor that constructs and empty message
        public Message()
        {
            this.header.action = (byte)Action.NO_ACTION;
            this.header.errorNo = (byte)0;
            this.header.dataLength = (int)0;
            this.data = new Byte[0];
        }

        public Message(MessageHeader header, Byte[] data)
        {
            this.header = header;
            this.data = new byte[data.Length + 1];
            for(int i =0; i< data.Length; i++)
            {
                this.data[i] = data[i];
            }
        }

        public Message(byte[] message)
        {
            this.header.action = message[0];
            this.header.errorNo = message[1];


            byte[] dataL = new byte[4];
            for(int i =0;i < 4; i++)
            {
                dataL[i] = message[2 + i]; 
            }

            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(dataL);
            }

            this.header.dataLength = BitConverter.ToInt32(dataL, 0);

            this.data = new byte[this.header.dataLength];

            for (int i = 0; i < this.header.dataLength; i++)
            {
                this.data[i] = message[6 + i];
            }
        }

        public Message(byte action, byte errorNo, int dataLength, byte[] data)
        {
            this.header.action = action;
            this.header.errorNo = errorNo;
            this.header.dataLength = dataLength;

            for (int i = 0; i < this.header.dataLength; i++)
            {
                this.data[i] = data[i];
            }
        }

        public Byte[] GetMessageAsByteArray()
        {
            //Converts the MessageHeader and the data into a byte array
            //6 from the sizeof(MessageHeader)

            Byte[] bytes = new Byte[ 6 + this.data.Length];
            bytes[0] = this.header.action;
            bytes[1] = this.header.errorNo;

            //Convert int into byte array
            byte[] intBytes = BitConverter.GetBytes(this.header.dataLength);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(intBytes);

            //Copy the array after conversion into the return array
            for(int i =0; i < intBytes.Length; i++)
            {
                bytes[2 + i] = intBytes[i];
            }

            //Copy the data length into the return array
            for(int i = 0; i < this.data.Length; i++)
            {
                bytes[6 + i] = this.data[i];
            }


            return bytes;
        }

        public Byte[] GetMessageData()
        {
            return this.data;
        }

        public byte GetMessageAction()
        {
            return this.header.action;
        }

        public byte GetMessageError()
        {
            return this.header.errorNo;
        }
    }
}
