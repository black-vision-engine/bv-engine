using System;
using System.Windows.Controls;
using System.Windows;


namespace ProfilerEditor.PresentationLayer
{
    public class TreeListView : TreeView
    {
        protected override DependencyObject GetContainerForItemOverride()
        {
            return new TreeListViewItem();
        }

        protected override bool IsItemItsOwnContainerOverride( object item )
        {
            return item is TreeListViewItem;
        }

        #region Public Properties

        /// <summary> GridViewColumn List</summary>
        public GridViewColumnCollection Columns
        {
            get
            {
                if ( m_columns == null )
                {
                    m_columns = new GridViewColumnCollection();
                }

                return m_columns;
            }
        }

        private GridViewColumnCollection m_columns;

        #endregion
    }

    public class TreeListViewItem : TreeViewItem
    {
        public int Level
        {
            get
            {
				if( m_level == -1 )
                {
                    TreeListViewItem parent = ItemsControl.ItemsControlFromItemContainer(this) as TreeListViewItem;
					m_level = ( parent != null ) ? parent.Level + 1 : 0;
                }
				return m_level;
            }
        }


        protected override DependencyObject GetContainerForItemOverride()
        {
            return new TreeListViewItem();
        }

        protected override bool IsItemItsOwnContainerOverride( object item )
        {
            return item is TreeListViewItem;
        }

        private int m_level = -1;
    }

}
