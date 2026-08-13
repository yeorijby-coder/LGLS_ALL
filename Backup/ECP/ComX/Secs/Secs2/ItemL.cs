using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using ECP.Util;

namespace ECP.ComX.Secs.Secs2
{
    public class ItemL : Item
    {
        public const String TYPE = "L";
        private static int INDENT = 2;

        private List<object> items_;

        private String name_ = null;

        public ItemL(String name)
        {
            name_ = name;
            Init(0);
        }

        public ItemL() :this("_no_name_")
        {

        }

        public ItemL(Item item1) : this("_no_name_")
        {
            Add(item1);
        }

        public ItemL(Item item1, Item item2) :this("_no_name_")
        {
     
            Add(item1);
            Add(item2);
        }

        public ItemL(Item item1, Item item2, Item item3) : this("_no_name_")
        {
         
            Add(item1);
            Add(item2);
            Add(item3);
        }

        public ItemL(Item item1, Item item2, Item item3,
                     Item item4): this("_no_name_")
        {
            
            Add(item1);
            Add(item2);
            Add(item3);
            Add(item4);
        }

        public ItemL(Item item1, Item item2, Item item3,
                     Item item4, Item item5) : this("_no_name_")
        {
            
            Add(item1);
            Add(item2);
            Add(item3);
            Add(item4);
            Add(item5);
        }

        public ItemL(Item item1, Item item2, Item item3,
                     Item item4, Item item5, Item item6) : this("_no_name_")
        {
            
            Add(item1);
            Add(item2);
            Add(item3);
            Add(item4);
            Add(item5);
            Add(item6);
        }

        public ItemL(Item item1, Item item2, Item item3,
                     Item item4, Item item5, Item item6,
                     Item item7): this("_no_name_")
        {

            Add(item1);
            Add(item2);
            Add(item3);
            Add(item4);
            Add(item5);
            Add(item6);
            Add(item7);
        }

        public ItemL(Item item1, Item item2, Item item3,
                     Item item4, Item item5, Item item6,
                     Item item7, Item item8): this("_no_name_")
        {
            Add(item1);
            Add(item2);
            Add(item3);
            Add(item4);
            Add(item5);
            Add(item6);
            Add(item7);
            Add(item8);
        }

        public ItemL(Item item1, Item item2, Item item3,
                     Item item4, Item item5, Item item6,
                     Item item7, Item item8, Item item9): this("_no_name_")
        {
            Add(item1);
            Add(item2);
            Add(item3);
            Add(item4);
            Add(item5);
            Add(item6);
            Add(item7);
            Add(item8);
            Add(item9);
        }

        
        public override void SetValue( Object o )
	    {
		    if( o is Item )
		    {
                items_ = new List<object>();
		        items_.Add( o );
		    }
	    }

	    
	    public override void SetValue( Object o, int index )
	    {
		    if( o is Item )
		    {
                items_[index] = o;

		    }
	    }

        public override void Init(int count)
        {
            items_ = new List<object>(count);
        }

        public override void Read(StreamTokenizer st)
        {
            try
            {
                while( true )
                {
				    // Look ahead to the next item
				    st.NextToken();

				    // If it is a begin character we have another item
                    if( st.ttype == SML.TT_BEGIN )
                    {
					    // Push it back on the stack
					    st.PushBack();

					    Item item = Item.GetNextItem( st );
					    Add( item );
                    }
				    // Have we reached the end of the list
				    else if( st.ttype == SML.TT_END )
				    {
					    break;
				    }
                    else
                    {
                        throw new FormatException(
							    "Incorrectly formated data for List Item" );
                    }
                }
            }
            catch( IOException ioe )
            {
                throw new FormatException("ItemL caught " + ioe);
            }
        }

        public override void Read(BinaryReader dis) 
        {
            int count = items_.Capacity;
            for (int i = 0; i < count; i++)
            {
                Add(Item.GetNextItem(dis));
            }
        }

        public override void Write(BinaryWriter dos)
        {
            WriteHeader(dos);
            int num_items = Count();
            for (int i = 0; i < num_items; i++)
            {
                Item item = (Item)items_[i];
                item.Write(dos);
            }
        }

        public override String Type()
        {
            return TYPE;
        }

	    
	    public override String ToIndentedEnglishString( int indent)
	    {
            StringBuilder sb = new StringBuilder();

            sb.Append(GetIndent(indent)).Append("List of ").Append(Count()).Append("\n");
            for( int i=0; i<Count(); i++ )
            {
                try
                {
                    sb.Append( Get(i).ToIndentedEnglishString( indent + 1) );
                }
                catch( Exception ex )
                {
                    // Failure creating message
                    sb.Append( "\n" ).Append(
                        ex.StackTrace.ToString() );
                    break;
                }
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
            sb.Append("<L[").Append(Count()).Append("]");
            for( int i=0; i<Count(); i++ )
            {
                try
                {
                    sb.Append(" " + Get(i).ToSMLString());
                }
                catch( Exception ex )
                {
                    // Failure creating SML message
                    sb.Append( "\n" ).Append(
                        ex.StackTrace.ToString() );
                    break;
                }
            }
            sb.Append( " >" );
            return sb.ToString();
        }

	   
	    public override String ToIndentedSMLString( int indent )
	    {
            StringBuilder sb = new StringBuilder();
            sb.Append(GetIndent(indent)).Append("<L[").Append(Count()).Append("]\n");
            for( int i=0; i<Count(); i++ )
            {
                try
                {
                    sb.Append( Get(i).ToIndentedSMLString( indent + 1 ) );
                }
                catch( Exception ex )
                {
                    // Failure creating SML message
                    sb.Append( "\n" ).Append(
                        ex.StackTrace.ToString() );
                    break;
                }
            }
            sb.Append(GetIndent(indent)).Append( ">" ).Append( '\n' );
            return sb.ToString();
	    }

        public override String ToString( int index )
        {
            return ToString();
        }
        public override String GetString()
        {
            return ToSMLString();
        }

        public override void Print(TextWriter pw, int indent)
        {
            base.Print(pw, indent);
            int num_items = Count();
            if (num_items == 0)
            {
                pw.WriteLine("<L>");
            }
            else
            {
                pw.WriteLine("<L");
                indent += INDENT;
                for (int i = 0; i < num_items; i++)
                {
                    Item item = (Item)items_[i];
                    if (item != null)
                    {
                        item.Print(pw, indent);
                    }
                }
                indent -= INDENT;
                base.Print(pw, indent);
                pw.WriteLine(">");
            }
        }

        public override void Add(Item item)
        {
            items_.Add(item);
        }

        public override void Remove(Item item)
        {
            int i = items_.IndexOf(item);
            if (i >= 0)
            {
                items_.RemoveAt(i);
            }
        }

        public override Object GetValue(int index)
        {
            return Get(index);
        }

        public Item Get(int index)
        {
            if (index >=0 && index < Count())
            {
                return (Item)items_[index];
            }
            else
            {
                return null;
            }
        }

        public void Set(Item item, int index)
        {
            if (index >=0 && index < Count())
            {
                items_[index] = item;
            }
        }

        public override Item Find(int[] locator)
        {
            int length = locator.Length;
            if (length > 0)
            {
                if (length == 1)
                {
                    return this;
                }
                else
                {
                    int[] new_locator = new int[length - 1];
                    for (int i = 1; i < length; i++)
                    {
                        new_locator[i-1] = locator[i];
                    }
                    Item item = Get(new_locator[0]);
                    if (item != null)
                    {
                        return item.Find(new_locator);
                    }
                    else
                    {
                        return null;
                    }
                }
            }
            else
            {
                return null;
            }
        }

        public String GetName()
        {
            return name_;
        }

        public void SetName(String name)
        {
            name_ = name;
        }

        public override int Count()
        {
            return items_.Count;
        }

        public override int SizeWithHeader()
        {
            int size = 0;
            int count = Count();
            for (int i = 0; i < count; i++)
            {
                Item item = (Item)items_[i];
                size += item.SizeWithHeader();
            }
            size += (NumLengthBytes(count) + 1);
            return size;
        }
    }
}
