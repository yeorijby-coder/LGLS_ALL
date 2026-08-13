using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using ECP.Util;

namespace ECP.ComX.Secs.Secs2
{
    abstract public class ItemN : Item
    {
        public override abstract int Count();

        public abstract void SetDatum(int i, double val);

        public abstract double GetDatum( int i );

        public override abstract void Init( int count);

        public virtual void ParseSML(StreamTokenizer st, bool integer, bool unsigned)
        {
            try
            {

                List<double> v = new List<double>();

                while (st.NextToken() == StreamTokenizer.TT_NUMBER)
                {
                    if (unsigned && st.nval < 0)
                    {
                        throw new FormatException("Unsigned cannot be negative");
                    }
                    double val = st.nval;
                    if (integer && (long)val != (double)val)
                    {
                        throw new FormatException("Integer cannot be fractional");
                    }
                    v.Add(val);
                }

                if (st.ttype == SML.TT_END)
                {
                    Init(v.Count);
                    int i = 0;

                    foreach (double d in v)
                    {
                        SetDatum(i++, d);
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
                throw new FormatException("caught " + ioe);
            }
        }

        public override void SetValue(Object o)
        {
            if(o is float || o is double || o is byte || o is short || o is int || o is long)
            {
                double val = (double)o;
			    SetDatum( 0, val );
            }
            else if( o is String )
            {
                try
                {
                    StreamTokenizer st =
                        new StreamTokenizer(new StringReader((String)o));

                    List<double> v = new List<double>();

                    while (st.NextToken() == StreamTokenizer.TT_NUMBER)
                    {
                        double val = st.nval;
                        v.Add(val);
                    }

                    Init(v.Count);
                    int i = 0;

                    foreach (double d in v)
                    {
                        SetDatum(i++, (double)d);
                    }



                }
                catch (IOException ioe)
                {
                    // Do nothing
                }
                catch (Exception e)
                {
                }
            }
            else
            {
			    SetValue( o.ToString() );
            }
        }

        public override void SetValue(Object o, int index)
        {
            if (o is float || o is double || o is byte || o is short || o is int || o is long)
            {
                double val = (double)o;
			    SetDatum( index, val );
            }
            else
            {
			    SetValue( o.ToString(), index );
            }
        }

        public override String GetString()
        {
            int num = Count();

            StringBuilder sb = new StringBuilder();

            for (int i = 0; i < num; i++)
            {
                if( i != 0 )
                {
                    sb.Append( " " );
                }
                sb.Append(ToString(i));
            }
            return sb.ToString();
        }

        public override String ToString()
        {
            return ToSMLString();
        }

        public override String ToSMLString()
        {

            StringBuilder sb = new StringBuilder();
            sb.Append("<" + Type());
            sb.Append(" " + GetString());
            sb.Append(">");
            return sb.ToString();
        }

        public int Size()
        {
            return NumBytesPerDatum() * Count();
        }

        public override int SizeWithHeader()
        {
            int size = Size();
            size += (NumLengthBytes(size) + 1);
            return size;
        }
    }
}
