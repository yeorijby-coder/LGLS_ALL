using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs1
{
    public class Secs1Message : SecsMessage
    {

        private List<byte[]> blockList = new List<byte[]>(5);

        
        public Secs1Message()
            :base(new Secs1Header())
        {
          
        }

        
        public Secs1Message(byte[] block)
            :base(new Secs1Header(block))
        {           
            AppendBlock(block);
        }

        
        public Secs1Message(bool rbit, SecsMessage secsMessage)
            :base(secsMessage.GetSecsHeader())
        {
          
            byte[] messageData = secsMessage.ToByteArray();

            // Update the and rbit in the message byte array.
            Secs1Header.SetRbit(messageData, rbit);

            FromByteArray(messageData);
        }

        
        public Secs1Message(int deviceId, bool rbit, SecsMessage secsMessage)
            :base(secsMessage.GetSecsHeader())
        {
            /*
             * This secs header is only temporary.  It mainly keeps us from
             * incrementing our transaction id until we can create the
             * Secs1Header.  We can't create it here because of limitations
             * on what you can do before calling the super.
            **/
            byte[] messageData = secsMessage.ToByteArray();

            // Update the device id and rbit in the message byte array.
            Secs1Header.SetDeviceId(messageData, deviceId);
            Secs1Header.SetRbit(messageData, rbit);

            FromByteArray(messageData);
        }

        
        public Secs1Message(Secs1Header secs1Header)
            :base(secs1Header)
        {
           
        }

       
        public Secs1Header GetSecs1Header()
        {
            return (Secs1Header)GetSecsHeader();
        }

       
        public void AppendBlock(byte[] block)
        {
            if (block != null)
            {
                blockList.Add(block);
            }
        }


        
        public byte[] GetBlock(int blockNum)
        {
            try
            {
                return (byte[])blockList[blockNum];
            }
            catch (ArgumentOutOfRangeException e)
            {
                return null;
            }
            catch (Exception e)
            {
                return null;
            }
        }

        
        public int GetNumBlocks()
        {
            return blockList.Count;
        }

       
        public override byte[] ToByteArray()
        {
            byte[] block;
            int msgDataLength = 0;

            msgDataLength = GetMessageSizeWithHeader();

            if (msgDataLength == 0)
            {
                return null;
            }

            byte[] msgData = new byte[msgDataLength];

            // copy header
            int cp = GetHeaderSize();
            Array.Copy(GetSecs1Header().ToByteArray(), 0, msgData, 0, cp);

            foreach(byte[] b in blockList)
            {
                block = b;
                if (block.Length > 10)
                {
                    Array.Copy(block, 10, msgData, cp, block.Length - 10);
                    cp += block.Length - 10;
                }
            }


            return msgData;
        }

       
        public override void FromByteArray(byte[] message)
        {
            byte[] block;
            int blockDataSize;
            int headerSize = GetHeaderSize();

            // Create and Initialize header.
            SetSecsHeader(new Secs1Header(message));
            SetBlockId(1);
            SetEbit(true);

            // Clear the block list (Should be empty anyway)
            blockList.Clear();


            // Calculate how many blocks we have
            int numBlocks = (message.Length - headerSize) / 244;
            if (((message.Length - headerSize) % 244) > 0 || numBlocks == 0)
            {
                numBlocks++;
            }

            // Loop threw each of the blocks;
            for (int i = 0; i < numBlocks; i++)
            {
                // How much room do we need for this block?
                blockDataSize = message.Length - headerSize - i * 244;
                if (blockDataSize > 244)
                {
                    blockDataSize = 244;
                }

                // Update the header to reflect the status of this block.
                SetBlockId(i + 1);
                if (i == numBlocks - 1)
                {
                    SetEbit(true);
                }
                else
                {
                    SetEbit(false);
                }

                // Create the block, copy header, copy data
                block = new byte[blockDataSize + headerSize];

                Array.Copy(HeaderToByteArray(), 0, block, 0, headerSize);

                if (blockDataSize > 0)
                {
                    Array.Copy(
                            message, i * 244 + 10, block, headerSize, blockDataSize);
                }

                AppendBlock(block);
            }


        }

        
        public override int GetMessageSize()
        {
            int size = 0;

            foreach (byte[] b in blockList)
            {
                byte[] block = b;
                size += block.Length;
                size -= GetHeaderSize(); // Subtract the header each time
            }           

            return size;
        }

        
        public bool GetRbit()
        {
            return GetSecs1Header().GetRbit();
        }

        
        public void SetRbit(bool value)
        {
            GetSecs1Header().SetRbit(value);
        }

       
        public bool GetEbit()
        {
            return GetSecs1Header().GetEbit();
        }

        
        public void SetEbit(bool value)
        {
            GetSecs1Header().SetEbit(value);
        }

        
        public int GetSessionId()
        {
            return GetSecs1Header().GetDeviceId();
        }

        
        public int GetDeviceId()
        {
            return GetSecs1Header().GetDeviceId();
        }

        
        public void SetDeviceId(int deviceId)
        {
            GetSecs1Header().SetDeviceId(deviceId);
        }

        
        public int GetBlockId()
        {
            return GetSecs1Header().GetBlockId();
        }

        
        public void SetBlockId(int blockId)
        {
            GetSecs1Header().SetBlockId(blockId);
        }
    }
}
