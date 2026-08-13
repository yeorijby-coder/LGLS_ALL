using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using ECP.Util;

namespace ECP.ComX.Secs.Secs2
{
    public class ItemB : ItemU1
    {
        private const String TYPE = "B";

        public ItemB() : base()
        {

        }

        public ItemB(byte datum) :base(datum)
        {

        }

        public ItemB(int datum):base(datum)
        {

        }

        public ItemB(byte[] data) : base(data)
        {
 
        }

        public override Object GetValue(int index) 
        {
            return base.GetValue(index);
        }

        public override void SetValue(Object o)
        {

            if( o is String )
            {
                String tmp = (String)o;

                // Does it look like we have some hex numbers
                if( -1 != tmp.IndexOf('x') )
                {
                    try
                    {
                        GetDataFromHexStream(
                                new StreamTokenizer(new StringReader( tmp )) );
                    }
                    catch( FormatException fe )
                    {
                        SetValue( (byte)0 );
                    }

                }
                else
                {
                    base.SetValue(o);
                }
            }
            else
            {
                base.SetValue(o);
            }
        }

        public override void ParseSML(StreamTokenizer st, bool integer, bool unSigned)
        {
            try
            {
                GetDataFromHexStream( st );

                // Get the TT_END character
                st.NextToken();
                if( st.ttype != SML.TT_END )
                {
                    throw new FormatException(
                            "Expected '" + (char)SML.TT_END + "'" );
                }
            }
            catch( IOException ioe )
            {
                throw new FormatException("caught " + ioe);
            }
        }

        private void GetDataFromHexStream( StreamTokenizer st )
        {
            st.OrdinaryChar( 'x' );
            st.OrdinaryChars( '0', '9' ); // Remove from number char
            st.WordChars( '0', '9' );     // Add to word char
            try
            {

                List<double> v = new List<double>();
                st.NextToken();
                // Note: the only reason it will be a number is if they previously
                // got this token and it was a number.
                while( st.ttype == StreamTokenizer.TT_WORD || st.ttype == StreamTokenizer.TT_NUMBER )
                {
                    // The first character MUST be the 0
                    if( (st.ttype==StreamTokenizer.TT_WORD && !st.sval.Equals( "0" )) ||
                            (st.ttype==StreamTokenizer.TT_NUMBER && (int)st.nval!=0) )
                    {
                        throw new FormatException(
                                "Binary hex values must begin with 0x" );
                    }

                    // The next character is the x
                    st.NextToken();
                    if( st.ttype != 'x' )
                    {
                        throw new FormatException(
                                "Binary hex values must begin with 0x" );
                    }

                    // Then follows the hex value
                    st.NextToken();



                    double val = (double)Convert.ToInt32(st.sval);
                    v.Add(val);

                    // Get the next token in prep
                    st.NextToken();
                }

                // We finished getting our data.  Let the caller pull off this
                // token.
                st.PushBack();

                // Fill out the byte[]
                Init(v.Count);
                int i = 0;

                foreach (double d in v)
                {
                    SetDatum(i++, d);
                }

                

            }
            catch (IOException ioe)
            {
                throw new FormatException("caught " + ioe);
            }
            finally
            {
                st.OrdinaryChars( '0', '9' ); // Remove from word char
                st.ParseNumbers();
                st.WordChars( 'x', 'x' );
            }
        }

        public  override String ToString(int i)
        {            
            int val = GetData()[i] & 0xFF;

            if( val < 16 )
            {
                return "0x0" + String.Format("{0:X}", val);  
            }
            else
            {
                return "0x" + String.Format("{0:X}", val);  
            }
        }

        public override String Type()
        {
            return TYPE;
        }
    }
}
