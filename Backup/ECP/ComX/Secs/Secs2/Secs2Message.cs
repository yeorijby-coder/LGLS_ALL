using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Reflection;
using System.Runtime.Remoting;


using ECP.Util;

namespace ECP.ComX.Secs.Secs2
{
    public class Secs2Message : SecsMessage
    {
        private String name;
        
        private Item messageBody;


        public Secs2Message() : this("", 0, 0, null)
        {
        }

        public Secs2Message(int stream, int function, Item item): this(null, stream, function, item)
        {         
            SetName(GetDefaultName());
        }

        public Secs2Message(String name, int stream, int function, Item item):base()
        {
   
            SetName(name);
            GetSecsHeader().SetStream( stream );
            GetSecsHeader().SetFunction( function );

            if (GetSecsHeader().IsPrimaryMessage())
            {
                // default behavior
                GetSecsHeader().SetWbit(true);
            }
            SetMessageBody(item);
        }

        public Secs2Message( SecsHeader secsHeader, Item item ):this(null, secsHeader, item )
        {
            
        }

        public Secs2Message( String name, SecsHeader header, Item item ):base(header)
        {
 
            SetName(name);
            SetMessageBody(item);
        }

        public Secs2Message( SecsMessage sm ):base (sm.GetSecsHeader() )
        {
            // Take advantage, if it is already a Secs2Message
            if( sm is Secs2Message )
            {
                SetMessageBody( ((Secs2Message)sm).GetMessageBody() );
            }
            else
            {
                FromByteArray( sm.ToByteArray() );
            }
            SetPrimaryMessage(sm.GetPrimaryMessage());
        }

        public Secs2Message( byte[] message ):base(message)
        {
            
            FromByteArray( message );
        }

        public String GetName()
        {
            if( name != null )
            {
                return name;
            }
            else
            {
                return "";
            }
        }

        public void SetName(String name)
        {
            this.name = name;
        }

        public String GetDefaultName()
        {
            return GetSecsHeader().GetDefaultName();
        }

        public void Read(StreamTokenizer st) 
        {
            Item item = null;
            String name = null;
            String strfun = null;
            try
            {
                while( true )
                {
                    st.NextToken();

                    if( st.ttype == StreamTokenizer.TT_WORD )
                    {
                        name = st.sval;
                        st.NextToken();
                        if( st.ttype == SML.TT_COLON )
                        {
                            st.NextToken();
                            if( st.ttype == StreamTokenizer.TT_WORD )
                            {
                                strfun = st.sval;
                            }
                        }
                        else if( st.ttype == StreamTokenizer.TT_WORD )
                        {
                            strfun = name;
                        }
                        else
                        {
                            strfun = name;
                            st.PushBack(); // the SML.TT_BEGIN token
                        }
                        st.NextToken();
                        if( st.ttype == StreamTokenizer.TT_WORD )
                        {
                            if( st.sval.Equals("W") )
                            {
                                GetSecsHeader().SetWbit( true );
                            }
                        }
                        else
                        {
                            st.PushBack(); // the SML.TT_BEGIN token
                        }
                    }
                    else if( st.ttype == SML.TT_BEGIN )
                    {
                        st.PushBack(); // let the get Item read its TT_Begin token

                        item = Item.GetNextItem(st);

                        st.ResetSyntax();  // make all characters special
                        st.WhitespaceChars(0, ' ');
                    }
                    else if( st.ttype == SML.TT_END )
                    {
                        throw new FormatException("Extraneous '" +
                                (char)SML.TT_END + "'");
                    }
                    else if( st.ttype == SML.TT_FINAL ||
                            st.ttype == StreamTokenizer.TT_EOF )
                    {
                        // restore default tokenizing
                        st.WordChars('a', 'z');
                        st.WordChars('A', 'Z');
                        st.WordChars(128 + 32, 255);
                        st.WordChars('_', '_');
                        st.CommentChar('*');
                        st.QuoteChar('"');
                        st.QuoteChar('\'');
                        st.ParseNumbers();
                        break; // out of while loop
                    }
                    else
                    {
                        Console.WriteLine(st);
                        
                    }
                }
            }
            catch (IOException ioe)
            {
                throw new FormatException("SML parsing failed");
            }

            // Accept null item(it may be a header only message)
            GetSecsHeader().SetSystemBytes();
            SetMessageBody(item);
            SetName(name);

            bool parsedSF = false;
            if (strfun.StartsWith("S"))
            {
                int fpos = strfun.IndexOf('F');
                if (fpos > 0)
                {
                    try
                    {
                        GetSecsHeader().SetStream(Convert.ToInt32(strfun.Substring(1,fpos)));
                        GetSecsHeader().SetFunction(
                                Convert.ToInt32(strfun.Substring(fpos+1)));
                        parsedSF = true;
                    }
                    catch (Exception nfe)
                    {
                    }
                }
            }
            if(!parsedSF)
            {
                throw new FormatException("Invalid Stream and/or Function");
            }
        }

        public void Read(BinaryReader dis) 
        {
            byte[] header = new byte[10];

            dis.Read(header, 0, 10);
            SetHeader(header);
            SetMessageBody(Item.GetNextItem(dis));
        }

        public override int GetMessageSize()
        {
            int item_size = 0;

            // Check for Messages with no Item i.e. S1F1
            if (GetMessageBody() != null)
            {
                item_size = GetMessageBody().SizeWithHeader();
            }

            return (item_size);
        }

        public String GetDescriptor()
        {
            return ( GetDefaultName() +
                (GetSecsHeader().GetWbit() ? " W" : "") +
                " #" + GetSecsHeader().GetSystemBytes());
        }

        public void Print(TextWriter pw)
        {
            pw.WriteLine(GetDescriptor());
            // Check for Messages with no Item i.e. S1F1
            if (GetMessageBody() != null)
            {
                GetMessageBody().Print(pw, 0);
            }
            pw.WriteLine((char)SML.TT_FINAL);
        }

        public String PrintToString()
        {
            StringBuilder bos = new StringBuilder();

            StringWriter pw = new StringWriter(bos);
            Print(pw);
            return bos.ToString();
        }

        public void Write(BinaryWriter dos) 
        {
            dos.Write(GetSecsHeader().ToByteArray(), 0, GetSecsHeader().GetSize());
            if (GetMessageBody() != null)
            {
                GetMessageBody(dos);
            }
        }

        public int[] ParseLocatorString(String locator)
        {
            List<int> v = new List<int>();

            
            StringTokenizer st = new StringTokenizer(locator, ".");
            while (st.hasMoreTokens())
            {
                v.Add(Convert.ToInt32(st.nextToken()));
            }
            int[] locator_array = new int[v.Count];

            for (int i = 0; i < v.Count; i++)
            {
                locator_array[i] = v[i];
            }
            return locator_array;
        }

        public Item Find(String locator)
        {
            return GetMessageBody().Find(ParseLocatorString(locator));
        }

        public void SetValue(String locator, Object _object)
        {
            Item item = Find(locator);
            if (item != null)
            {
                item.SetValue(_object);
            }
        }

       
        public Secs2Message GetGEM()
        {
            String gems = Name.GetPackagePrefix(GetType().FullName) +
                          "Gem." + GetDefaultName();

            Secs2Message rtnval = (Secs2Message)Name.NewInstance(gems);
            return (rtnval != null ? rtnval : this);
        }

        
        public static  Secs2Message NewGEMInstance( SecsMessage sm )
        {
            //TO-DO Fix later
            String gems = "ECP.ComX.Secs.Secs2."+"Gem";
            return NewSpecificInstance( gems, sm );
        }
        
        
        public static  Secs2Message NewSpecificInstance( 
            String packageName, SecsMessage sm )
        {
            Secs2Message rtnval = null;
        
            String className = packageName + "." + 
                sm.GetSecsHeader().GetDefaultName();
        
            Object[] args = new Object[] { sm }; 

            Assembly asembly = null;

            try
            {
                asembly = Assembly.Load("ECP");
            }
            catch (FileNotFoundException ex)
            {
                Console.WriteLine(ex.Message.ToString());
            }

            try
            {
                Type type = asembly.GetType(className);          
                object obj = Activator.CreateInstance(type,args);

                rtnval = obj as Secs2Message;
                rtnval.FromByteArray( sm.ToByteArray() );
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.Message.ToString());
            }
       
            return rtnval;
        }


        public virtual Secs2Message GetSecondary()
        {
            return new Secs2Message( GetSecsHeader().NewSecondaryHeader(), null );
        }


        
        public bool Matches(Secs2Message other)
        {
            if( GetSecsHeader().GetStream() == other.GetSecsHeader().GetStream() &&
                GetSecsHeader().GetFunction() ==
                                            other.GetSecsHeader().GetFunction() )
            {
                return true;
            }
            return false;
        }

        public override String ToString()
        {
            StringBuilder buf = new StringBuilder();
            String desc = GetDescriptor();

            for (int i=0; i<desc.Length; i++)
            {
                char c = desc[i];
                buf.Append((c==' ') ? '/' : c);
            }

            if( GetMessageBody() != null )
            {
                buf.Append( GetMessageBody().ToString() );
            }

            return buf.ToString();
        }

        public String ToSMLString()
        {
            StringBuilder sb = new StringBuilder();

            sb.Append( GetSecsHeader().ToString() );
            if( GetMessageBody() != null )
            {
                sb.Append( GetMessageBody().ToSMLString() );
            }

            return sb.ToString();
        }

        public String ToIndentedSMLString()
        {
            StringBuilder sb = new StringBuilder();

            sb.Append( GetSecsHeader().ToString() ).Append( '\n' );
            if( GetMessageBody() != null )
            {
                sb.Append( GetMessageBody().ToIndentedSMLString() );
            }

            return sb.ToString();
        }

       
        public String MessageBodyToSMLString()
        {
            if( GetMessageBody() != null )
            {
                return GetMessageBody().ToSMLString();
            }
            else
            {
                return "";
            }
        }

        
        public static void SetupSMLStreamTokenizer( StreamTokenizer st )
        {
            st.WordChars('_', '_');
            st.OrdinaryChar( '/' );
        }

        
        public void MessageBodyFromSMLString( String smlString )
        {
            StreamTokenizer st =
                new StreamTokenizer( new StringReader(smlString) );

            SetMessageBody( ItemFromSML(st) );
        }

        
        public static Item ItemFromSML( StreamTokenizer st )
        {
            SetupSMLStreamTokenizer( st );

            try
            {
                // Is there at least something in the string?
                st.NextToken();

                // Yes - is it a begin character ('<')
                if( st.ttype == SML.TT_BEGIN )
                {
                    st.PushBack();


                    // get the messageBody
                    Item item = Item.GetNextItem(st);
                    if( item == null )
                    {
                        throw new FormatException(
                                "Unknown SECS type " + st.sval );
                    }
                    return item;
                }
                // NO - there was no body
                else if( st.ttype == StreamTokenizer.TT_EOF )
                {
                    return null;
                }
                else
                {
                    throw new FormatException(
                            "SML string must begin with the < character" );
                }
            }
            // I don't believe this exception is possible with a string, but ...
            catch( IOException ex )
            {
                throw new FormatException(
                        "An I/O exception occured parsing SML message body. " +
                        ex );
            }
        }

        public void SetMessageBody( BinaryReader dis  )
        {
            messageBody = Item.GetNextItem(dis);
        }

        public void GetMessageBody( BinaryWriter dos ) 
        {
            GetMessageBody().Write(dos);
        }

        public override byte[] ToByteArray()
        {

            MemoryStream baos = new MemoryStream(GetMessageSizeWithHeader());

            try
            {
                Write(new BinaryWriter(baos));
            }
            catch( IOException e )
            {
                Console.WriteLine(e.StackTrace);
            }
            return baos.ToArray();
            
        }

        public override void FromByteArray( byte[] message )
        {
            MemoryStream bais = new MemoryStream(message);
            try
            {
                Read(new BinaryReader(bais) );
            }
            catch( IOException e )
            {
                Console.WriteLine(e.StackTrace);
            }
        }

        public Item GetMessageBody()
        {
            return messageBody;
        }

        public void SetMessageBody(Item item)
        {
            this.messageBody = item;
        }
    }
}
