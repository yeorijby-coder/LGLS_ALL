using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using ECP.Util;


namespace ECP.ComX.Secs.Secs2
{
    public class ItemBOOLEAN : Item
    {
        public const String TYPE = "BOOLEAN";


        private bool [] data_;

	    public bool[] GetData()
	    {
		    return data_;
	    }

	    public void SetData( bool[] data )
	    {
		    data_ = data;
	    }

        public ItemBOOLEAN() : this(false)
        {

        }

        public ItemBOOLEAN(bool datum)
        {
		    SetValue( datum );
        }

        public ItemBOOLEAN( bool[] data )
        {
		    SetValue( data );
        }

        public ItemBOOLEAN( Object data )
        {
		    SetValue( data );
        }

	    public void SetValue( bool datum )
	    {
		    data_ = new bool[] { datum };
	    }

	    public void SetValue( bool[] data )
	    {
		    data_ = data;
	    }

        public ItemBOOLEAN(char datum)
        {
            if (datum == 'T')
            {
                SetValue( true );
            }
            else if (datum == 'F')
            {
			    SetValue( false );
            }
            else
            {
                // should we throw FormatException here?
			    SetValue( false );
            }
        }

        public override void SetValue(Object o)
        {
            if (o is bool)
            {
                 SetValue( (bool)o);
            }
            else if( o is String )
            {
               
                if( ((String)o).Equals("0") ||
                    ((String)o).Equals("false", StringComparison.OrdinalIgnoreCase))
                {
                    SetValue( false );
                }
                else
                {
                    SetValue( true );
                }
            }
            else
            {
                SetValue( o.ToString() );
            }
        }

        public override void SetValue( Object o, int index )
        {
            if (o is bool)
            {
			    data_[index] = (bool)o;
            }
        }

        public  override Object GetValue(int index)
        {
            return (data_[index]);
        }


        public override void Init(int count)
        {
		    SetData( new bool[count] );
        }

        public override void Read(StreamTokenizer st)
        {
		    ParseSML( st );
        }

	    public void ParseSML( StreamTokenizer st )
	    {
            try
            {

                List<bool> v = new List<bool>();
			    while( st.NextToken() == StreamTokenizer.TT_WORD )
			    {
				    String datum = st.sval;
				    bool val;

				    if( datum.Equals("T") )
				    {
					    val = true;
				    }
				    else if( datum.Equals("F") )
				    {
					    val = false;
				    }
				    else
				    {
					    throw new FormatException(
							    "Boolean values must be `T' or `F'" );
				    }

                    v.Add(val);

			    }

			    if( st.ttype == SML.TT_END )
			    {
				    Init( v.Count );
				    int i = 0;


                    foreach(bool b in v)
                    {
                        data_[i++] = b;
                    }


				    
			    }
			    else
			    {
				    throw new FormatException(
						    "Expected '" + (char)SML.TT_END + "'" );
			    }
            }
            catch (IOException ioe)
            {
                throw new FormatException( "caught " + ioe );
            }
	    }

        public override void Read(BinaryReader dis)
        {
		    byte[] data = new byte[Count()];

		    dis.Read( data, 0, Count() );
		    for( int i=0; i<data.Length; i++ )
		    {
			    if( data[i] == 0 )
			    {
				    data_[i] = false;
			    }
			    else
			    {
				    data_[i] = true;
			    }
		    }
        }

        public override void Write(BinaryWriter dos)
        {
		    byte[] data = new byte[Count()];
		    for( int i=0; i<data_.Length; i++ )
		    {
			    if( data_[i] == true )
			    {
				    data[i] = 1;
			    }
			    else
			    {
				    data[i] = 0;
			    }
		    }

		    WriteHeader(dos);
		    dos.Write( data, 0, data.Length );
        }

        public override String Type()
        {
            return TYPE;
        }

        public override int Count()
        {
            return (data_ == null ) ? 0 : data_.Length;
        }

        public void SetDatum(int i, bool val)
        {
            if( data_ == null )
            {
                Init( 1 );
            }
            data_[i] = val;
        }

        public bool GetDatum( int i )
        {
		    return data_[i];
        }

        public override String GetString()
        {

            StringBuilder sb = new StringBuilder();
		    for( int i=0; i<Count(); i++ )
		    {
			    if( i != 0 )
			    {
				    sb.Append( " " );
			    }
			    sb.Append( ToString(i) );
		    }
		    return sb.ToString();
        }

	    public String GetString(int i)
	    {
            return ToString(i);
	    }

        public String GetSMLString()
        {
            StringBuilder sb = new StringBuilder();

		    for( int i=0; i<Count(); i++ )
		    {
			    if( i != 0 )
			    {
				    sb.Append( " " );
			    }
			    sb.Append( GetSMLString(i) );
		    }
		    return sb.ToString();
        }

	    public String GetSMLString(int i)
	    {
		    return data_[i] ? "T" : "F";
	    }

        public override String ToSMLString()
        {
            return ("<" + Type() + " " + GetSMLString() + ">");
        }

        public override String ToString( int i )
        {
		    return data_[i] ? "1" : "0";
        }

        public override int SizeWithHeader()
        {
            int size = Count();
            size += (NumLengthBytes(size) + 1);
            return size;
        }
    }
}
