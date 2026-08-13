using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using ECP.Util;

namespace ECP.ComX.Secs.Secs2
{
    abstract public class Item
    {

        static Dictionary<string, int> format_codes = null;
        static Dictionary<int,string> codes_format = null;

        static String fully_qualified_class_name =
            Name.GetPackagePrefix((new FormatException2()).GetType().FullName) + "Item";

        

        static Item()
        {
            

            format_codes = new Dictionary<string,int>();       // hex (NLB==1)
            //format_codes.Add("L", 000);       // 01
            //format_codes.Add("B", 010);       // 21
            //format_codes.Add("BOOLEAN", 011); // 25
            //format_codes.Add("A", 020);       // 41
            //format_codes.Add("J", 021);       // 45
            //format_codes.Add("I8", 030);      // 61
            //format_codes.Add("I1", 031);      // 65
            //format_codes.Add("I2", 032);      // 69
            //format_codes.Add("I4", 034);      // 71
            //format_codes.Add("F8", 040);      // 81
            //format_codes.Add("F4", 044);      // 91
            //format_codes.Add("U8", 050);      // A1
            //format_codes.Add("U1", 051);      // A5
            //format_codes.Add("U2", 052);      // A9
            //format_codes.Add("U4", 054);      // B1

            format_codes.Add("L", 0);       // 01
            format_codes.Add("B", 8);       // 21
            format_codes.Add("BOOLEAN", 9); // 25
            format_codes.Add("A", 16);       // 41
            format_codes.Add("J", 17);       // 45
            format_codes.Add("I8", 24);      // 61
            format_codes.Add("I1", 25);      // 65
            format_codes.Add("I2", 26);      // 69
            format_codes.Add("I4", 28);      // 71
            format_codes.Add("F8", 32);      // 81
            format_codes.Add("F4", 36);      // 91
            format_codes.Add("U8", 40);      // A1
            format_codes.Add("U1", 41);      // A5
            format_codes.Add("U2", 42);      // A9
            format_codes.Add("U4", 44);      // B1

            codes_format = new Dictionary<int,string>();

            foreach(String type in format_codes.Keys)
            {
                int code = FormatCode(type);
                codes_format.Add(code, type);
            }

        }

        public static int FormatCode(String type)
        {
            if (format_codes.ContainsKey(type))
            {
                return format_codes[type];
            }
            else
            {
                return -1;
            }            
        }

        public static String CodeFormat(int code)
        {
            if (codes_format.ContainsKey(code))
            {
                return codes_format[code];
            }
            else
            {
                return "";
            }
        }

        public virtual String Type()
        {
            return "";
        }

        public override String ToString()
        {
            return Type();
        }

        abstract public String ToString( int index );
        abstract public String ToSMLString();

        protected String shiftWidth = "  ";

        public virtual String ToIndentedEnglishString()
        {
            return ToIndentedEnglishString(0);
        }

        public virtual String ToIndentedEnglishString( int indent)
        {
            return GetIndent( indent ) + "'" + GetString() + "'\n";
        }

        public virtual String ToIndentedSMLString()
        {
            return ToIndentedSMLString( 0 );
        }

        public virtual String ToIndentedSMLString( int indent )
        {
            return GetIndent( indent ) + ToSMLString() + '\n';
        }

        protected String GetIndent( int indent )
        {

            StringBuilder sb  = new StringBuilder();

            for( int i=0; i<indent; i++ )
            {
                sb.Append( shiftWidth );
            }

            return sb.ToString();

            
        }


        public virtual void Print(TextWriter pw, int indent)
        {
            for (int i=0; i < indent; i++) {pw.Write(' ');}
            String s = ToString();
            if (s.Equals(""))
            {
                pw.Write(s);
            }
            else
            {
                pw.WriteLine(s);
            }
        }

        public virtual void Add(Item item)
        {
        }

        public virtual void Remove(Item item)
        {
        }

        public virtual Item Find(int[] locator)
        {
            return (locator.Length == 1) ? this : null;
        }

        
        public Object GetValue()
        {
            return GetValue(0);
        }

        
        abstract public Object GetValue(int index);

        
        abstract public void SetValue(Object o);


        
        abstract public void SetValue(Object o, int index);

        public static Item GetItem(String itemType)
        {
            return (Item)Name.NewInstance(fully_qualified_class_name + itemType);
        }

        public static Item GetNextItem(BinaryReader dis)
        {           
            Item item = null;
            byte formatNLB;
            try
            {
                formatNLB = (byte)dis.ReadByte();
            }
            catch (Exception e)
            {
                return null;
            }

            int b1,b2,b3;
            int num_len_bytes = formatNLB & 0x03;
            int format = ((formatNLB & 0xff) >> 2);
            int len = 0;

            switch (num_len_bytes)
            {
			    case 1:
				    len = dis.ReadByte();
				    len = len & 0xFF;
				    break;
			    case 2:
				    b1 = dis.ReadByte();
				    b1 =  b1 & 0xFF;
				    b1 = b1 << 8;

				    b2 = dis.ReadByte();
				    b2 =  b2 & 0xFF;

				    len = b1 + b2;
				    break;
			    case 3:
				    b1 = dis.ReadByte();
				    b1 =  b1 & 0xFF;
				    b1 = b1 << 16;

				    b2 = dis.ReadByte();
				    b2 =  b2 & 0xFF;
				    b2 = b2 << 8;

				    b3 = dis.ReadByte();
				    b3 =  b3 & 0xFF;

				    len = b1 + b2 + b3;
				    break;
			    default:
				    throw new IOException(
						    "invalid number of length bytes format = ");// +
						   // Integer.toHexString(format) +
						  //  " num length bytes " + num_len_bytes );
		    }

            String type = CodeFormat(format);
		    // Console.WriteLine( type + " " + len );
            item = GetItem(type);
            if (item != null)
            {
                // len is number of bytes, so must scale it down
                len /= item.NumBytesPerDatum();
                item.Init(len);                
                item.Read(dis);

                
            }
            else
            {
			    throw new IOException(
					    "Invalid item type: " + type + " Format value: " + format +
					    " Length: " + len );
            }
            return item;
        }

	    
        public static Item GetNextItem(StreamTokenizer st)
        {
    		
		    try
		    {
			    st.NextToken();
			    if( st.ttype != SML.TT_BEGIN )
			    {
				    throw new FormatException(
						    "SML Items must begin with a '<' character" );
			    }

			    st.NextToken();
			    if( st.ttype != StreamTokenizer.TT_WORD )
			    {
				    throw new FormatException(
						    "SML message contained a '<' character " +
						    "without a following item type." );
			    }

			    // Get an Item based on the item type.
			    Item item = GetItem(st.sval);
			    if( item == null )
			    {
				    throw new FormatException(
						    "Unknown SECS type " + st.sval );
			    }

			    // Read off anything between the '[]' brackets. We don't need
			    // it if it is there.
			    st.NextToken();
			    if( st.ttype == SML.TT_OPEN )
			    {
				    do
				    {
					    st.NextToken();
				    } while( st.ttype != SML.TT_CLOSE );
				    // We just read the ] character, next token will be data
			    }
			    else
			    {
				    // We got data when looking for the [ character, push it
				    // back on.
				    st.PushBack();
			    }

			    // Read the data
			    item.Read(st);

			    return item;
		    }
		    catch (IOException ioe)
		    {
			    throw new FormatException("caught " + ioe);
		    }
        }

        public virtual int Count()
        {
            return 0;
        }

        public virtual int NumBytesPerDatum()
        {
            return 1;
        }

        public void WriteHeader(BinaryWriter dos)
        {
            
            int len = (Count() * NumBytesPerDatum());
            int num_len_bytes = NumLengthBytes(len);
            int format_code = FormatCode(Type());
            byte one = (byte)((format_code << 2) + num_len_bytes);

            dos.Write(one);

            switch (num_len_bytes)
            {
            case 1:
                dos.Write((byte)len);             
                break;
            case 2:
                dos.Write((short)len);
                break;
            case 3:
                dos.Write((byte)((len >> 16) & 0xFF));
                dos.Write((byte)((len >> 8) & 0xFF));
                dos.Write((byte)((len >> 0) & 0xFF));
                break;
            default:
                throw new IOException(); // should throw a different exception here
            }
        }

        public static int NumLengthBytes(int num_bytes)
        {
            if (num_bytes < (1 << 8))
            {
                return 1;
            }
            else if (num_bytes < (1 << 16))
            {
                return 2;
            }
            else if (num_bytes < (1 << 24))
            {
                return 3;
            }
            else
            {
                return 0;
            }
        }

	    
        public abstract void Read(StreamTokenizer st);

        public abstract void Read(BinaryReader dis);

        public abstract void Write(BinaryWriter dos);

        public abstract int SizeWithHeader();

        public abstract void Init(int count);

        public abstract String GetString();
    }
}
