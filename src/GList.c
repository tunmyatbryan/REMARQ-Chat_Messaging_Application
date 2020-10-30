/***************************************************
 * Module for Viewing List, Editing Profile, Window
 * 
 * Author: Bianca Doronila
 * Created: 03/01/18
 *
 * Modifications:
 *
 ***************************************************/

#include <gtk/gtk.h>
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Register.h"
#include "Login.h"
#include "Functions.h"
#include "DataStructures.h"

/* list tree */
enum 
{
  LIST_ITEM = 0,
  N_COLUMNS
};

// Window variables
GtkWidget *ListWindow;

// HBox, VBox, and Table variables
GtkWidget *HBox1; 
GtkWidget *ChatBox, *ButtBox, *vsep;
GtkWidget *ButtTable;

// Button variables
GtkWidget *EditButt, *StatusButt, *AddButt, *DltButt, *ChatButt,
				  *AddLbl , *DltLbl,  *ChatLbl;
GtkWidget *Notif, *Upd;

// Label Variables
GtkWidget *Online;
// TextView Variables
GtkWidget *OnlineView;
GtkTreeSelection *selection;
GtkWidget *OnlineScroll, *label;

// Modify Font
PangoFontDescription *df;

// Chat Variables
GtkWidget *VBox1;
GtkWidget *ChatFrame, *ChatView;
GtkWidget *hsep, *ChatScroll;
GtkWidget *ChatTable, *ChatEntry, *SENDButton;

//Texting variables
const char  *forDeleteText;
const gchar *TypedText;
GtkTextBuffer *TextBuffer;
GtkTextIter iter;

static gboolean run = FALSE;
static gboolean start = FALSE;
static int interval = 0;

/* changes the buffer for highlighted friend */
void on_changed(GtkWidget *widget, gpointer label) {
    
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
    gtk_label_set_text(GTK_LABEL(label), value);
    forDeleteText = value;
    #ifdef GMAIN
	g_print("Username highlighted: %s\n", forDeleteText);
    #endif
  }
} 

/* responsible for adding to your friend tree and manipulating txt file */
void add_to_list(GtkWidget *list, const gchar *str) {
    
  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model
      (GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}

/* initializes your friends list if it exists already */
void init_list(GtkWidget *list)
{

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore       *store;

  gchar *user;

  user = (char*)malloc(20*sizeof(gchar)); 
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes("Friend List",
          renderer, "text", LIST_ITEM, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

  store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
      GTK_TREE_MODEL(store));

  g_object_unref(store);

  FILE *File = fopen("SampleFriends.txt", "r");
  
  if(File != NULL)
  {
  while(!feof(File))
  {
	fscanf(File, "%19s\n", user);       
	add_to_list(list, user);
  }
  fclose(File);
  }
}


// Edit Button event
static void editbutton_clicked(GtkButton *EditButt, GtkWindow *ListWindow)
{
//DialogBox Variables for EDIT
	GtkWidget *EditDialog, *EditTable, *First, *Last, *DOB;
	GtkWidget *FirstLbl, *LastLbl, *DOBLbl;

	// create dialog
	EditDialog = gtk_dialog_new_with_buttons("Edit Profile", ListWindow, GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

	FirstLbl = gtk_label_new("First Name: ");
	LastLbl  = gtk_label_new("Last  Name: ");
	DOBLbl   = gtk_label_new("Date of Birth: ");

	First = gtk_entry_new();
	Last  = gtk_entry_new();	
	DOB   = gtk_entry_new();

	gtk_entry_set_text(GTK_ENTRY(First), "Current First Name");
	gtk_entry_set_text(GTK_ENTRY(Last), "Current Last Name");
	gtk_entry_set_text(GTK_ENTRY(DOB), "MMDDYYYY");

	EditTable = gtk_table_new(2, 3, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(EditTable), FirstLbl, 0, 1, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(EditTable),  LastLbl, 0, 1, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(EditTable),  DOBLbl, 0, 1, 2, 3);
	

	gtk_table_attach_defaults(GTK_TABLE(EditTable), First, 1, 2, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(EditTable),  Last, 1, 2, 1, 2);	
	gtk_table_attach_defaults(GTK_TABLE(EditTable),   DOB, 1, 2, 2, 3);
	
	gtk_table_set_row_spacings (GTK_TABLE (EditTable), 5);
  	gtk_table_set_col_spacings (GTK_TABLE (EditTable), 5);
  	gtk_container_set_border_width (GTK_CONTAINER (EditTable), 5);
  	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (EditDialog)->vbox), EditTable);
 
	gtk_window_set_resizable(GTK_WINDOW(EditDialog), FALSE);	
 	gtk_widget_show_all(EditDialog);

	gtk_dialog_run (GTK_DIALOG (EditDialog));
	gtk_widget_destroy (EditDialog);
}

// Add Button event
static void addbutton_clicked(GtkButton *AddButt, GtkWindow *ListWindow)
{
//DialogBox Variables for ADD
	GtkWidget *AddDialog, *AddTable, *Username;
	GtkWidget *UserLbl;
	gint result;
	
	gint n;
	const gchar *value, *valuematch;	
  	
	value  = (char*)malloc(20*sizeof(gchar)); 
	valuematch = (char*)malloc(20*sizeof(gchar)); 
	
	// create dialog
	AddDialog = gtk_dialog_new_with_buttons("Add a Friend", ListWindow, GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

	UserLbl = gtk_label_new("Username: ");
	Username = gtk_entry_new();

	gtk_entry_set_text(GTK_ENTRY(Username), "username");

	AddTable = gtk_table_new(1, 2, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(AddTable), UserLbl, 0, 1, 0, 1);

	gtk_table_attach_defaults(GTK_TABLE(AddTable), Username, 1, 2, 0, 1);
	
	gtk_table_set_row_spacings (GTK_TABLE (AddTable), 5);
 	gtk_table_set_col_spacings (GTK_TABLE (AddTable), 5);
  	gtk_container_set_border_width (GTK_CONTAINER (AddTable), 5);
  	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (AddDialog)->vbox), AddTable);
 
	gtk_window_set_resizable(GTK_WINDOW(AddDialog), FALSE);	
 	gtk_widget_show_all (AddDialog);

	result = gtk_dialog_run (GTK_DIALOG (AddDialog)); 
	if (result == GTK_RESPONSE_OK)
 	{

	value = gtk_entry_get_text(GTK_ENTRY(Username));
  	
	FILE *File;
  	File = fopen("SampleFriends.txt", "a+");

	while(!feof(File))
	{
	fscanf(File,"%s\n", valuematch);
	#ifdef GMAIN	
	printf("valuematch: %s\n", valuematch);
	printf("value     : %s\n", value);
	#endif
	if(strcmp(value, valuematch) == 0)
	{
		g_print("username already added.\n");
		n = 1;
		break;
	}
	}
	
	if(n==0)
	{
		fprintf(File, "%s\n", value);
		g_print("successfully added %s\n", gtk_entry_get_text(GTK_ENTRY(Username)));
		add_to_list(OnlineView, value);
	}
		fclose(File);
	}
	gtk_widget_destroy (AddDialog);
}

// Delete Button event
static void dltbutton_clicked(GtkButton *DltButt, GtkWindow *ListWindow)
{
//DialogBox Variables for DLT
	GtkWidget *DltDialog, *DltTable, *Username;
	GtkWidget *UserLbl;
	gint result;

	// create dialog
	DltDialog = gtk_dialog_new_with_buttons("Delete a Friend", ListWindow, GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

	UserLbl = gtk_label_new("Username: ");
	Username = gtk_entry_new();

	if (forDeleteText == NULL)
	{
	#ifdef GMAIN
	g_print("no username selected.");
	#endif
	gtk_entry_set_text(GTK_ENTRY(Username), "username");
	}
	else
	{
	#ifdef GMAIN
	g_print("username entry for deletion: %s\n", forDeleteText);
	#endif
	gtk_entry_set_text(GTK_ENTRY(Username), forDeleteText);
	}

	DltTable = gtk_table_new(1, 2, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(DltTable), UserLbl, 0, 1, 0, 1);

	gtk_table_attach_defaults(GTK_TABLE(DltTable), Username, 1, 2, 0, 1);
	
	gtk_table_set_row_spacings (GTK_TABLE (DltTable), 5);
 	gtk_table_set_col_spacings (GTK_TABLE (DltTable), 5);
  	gtk_container_set_border_width (GTK_CONTAINER (DltTable), 5);
  	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (DltDialog)->vbox), DltTable);

	gtk_window_set_resizable(GTK_WINDOW(DltDialog), FALSE);	
  	gtk_widget_show_all (DltDialog);

	result = gtk_dialog_run (GTK_DIALOG (DltDialog)); 
	if (result == GTK_RESPONSE_OK)
 	{

    	g_print("HELLO! THIS FUNCTION IS NOT DONE... To be deleted:  %s\n", gtk_entry_get_text(GTK_ENTRY(Username)));
	}
	gtk_widget_destroy (DltDialog);
}

// Status Button event
static void statusbutton_clicked(GtkButton *StatusButt, GtkWindow *ListWindow)
{
//DialogBox Variables for STATUS
	GtkWidget *StatusDialog;
	GtkWidget *box1, *box2;
	GtkWidget *AvailButt, *InvButt, *IdleButt, *separator;
	GSList *group;
	
	// create dialog
	StatusDialog = gtk_dialog_new_with_buttons("Update Status", ListWindow, GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

	/* main box pack onto window of status dialog */
	box1 = gtk_vbox_new(FALSE, 0);

    	box2 = gtk_vbox_new (FALSE, 10);
    	gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
    	gtk_box_pack_start (GTK_BOX (box1), box2, TRUE, TRUE, 0);

    	AvailButt = gtk_radio_button_new_with_label (NULL, "Available");
    	gtk_box_pack_start (GTK_BOX (box2), AvailButt, TRUE, TRUE, 0);
	gtk_widget_show(AvailButt);

    	group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(AvailButt));
    	
	IdleButt = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(AvailButt), "Idle");
    	gtk_box_pack_start (GTK_BOX (box2), IdleButt, TRUE, TRUE, 0);

    	InvButt = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(AvailButt), "Invisible");
    	gtk_box_pack_start (GTK_BOX (box2), InvButt, TRUE, TRUE, 0);

    	separator = gtk_hseparator_new ();
    	gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 0);
    	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(StatusDialog)->vbox), box1);

	gtk_window_set_resizable(GTK_WINDOW(StatusDialog), FALSE);	
	gtk_widget_show_all(StatusDialog);

	gtk_dialog_run(GTK_DIALOG(StatusDialog));
	gtk_widget_destroy (StatusDialog);
}

// Close Window event
static gboolean on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	interval = -1;
	run = FALSE;
	start = FALSE;

	g_print("Successfully signed out.\n");
	gtk_main_quit();
	return TRUE;
}

//User types in text box
static void UserText(GtkWidget *widget, GtkEntry *entry)
{
	#ifdef GMAIN	
	TypedText = gtk_entry_get_text(entry);	
	printf("the message is: %s\n", TypedText);
	#endif 
}

//send button clicked
static void SendButton_Clicked(GtkWidget *widget, gpointer data)
{

	GtkTextMark *mark;
	GtkTextIter iter;
	const gchar *text;

	TextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ChatView));
	text = gtk_entry_get_text(GTK_ENTRY(ChatEntry));
	
	mark = gtk_text_buffer_get_insert(TextBuffer);

	gtk_text_buffer_get_iter_at_mark(TextBuffer, &iter, mark);
	if(gtk_text_buffer_get_char_count(TextBuffer))
	{
		gtk_text_buffer_insert(TextBuffer, &iter, "\n\n", 1);
	}

	gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(ChatView), mark);
	gtk_text_buffer_insert(TextBuffer, &iter, text, -1);

}

static gboolean ServerUpdate(gpointer data)
{
char buf[256];
memset(&buf, 0x0, 256);
++interval;

snprintf(buf, 255, "Interval is now: %d\n", interval);

//g_print("%s", buf);
return run;
}

void ServerRun()
{
	if(!start)
	{
		//g timer
		g_timeout_add(1000, ServerUpdate, NULL);
		
		start = TRUE;
		run   = TRUE;
	}
}

GtkWidget *CreateListWin(int *argc, char **argv[])
{

	/*initialize from the server*/
//	ServerRun();

	gtk_init(argc, argv);

	df = pango_font_description_from_string("Samanata Bold");
	pango_font_description_set_size(df, 13*PANGO_SCALE);
	
	ListWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(ListWindow, 800, 400);
	gtk_container_set_border_width(GTK_CONTAINER(ListWindow), 5);
	gtk_window_set_position(GTK_WINDOW(ListWindow), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(ListWindow),"Remarq: Chat");
	gtk_window_set_resizable(GTK_WINDOW(ListWindow),TRUE);

	/* main box */
	HBox1 = gtk_hbox_new(TRUE, 3);
	gtk_container_add(GTK_CONTAINER(ListWindow), HBox1);

	/* seperate the button box and chat box */
	vsep    = gtk_vseparator_new();

	/*hbox for chat components, right side*/
	ChatBox = gtk_hbox_new(TRUE, 1); 
	/*hbox for button components, left side*/
	ButtBox = gtk_hbox_new(TRUE, 1);

	/* creating labels */
	Online  = gtk_label_new("online : ");
	gtk_widget_modify_font(Online, df);
	
	// creating tree views
	OnlineView   = gtk_tree_view_new();
		
	// pack buttons into a table and into the Button HBOX	
	EditButt = gtk_button_new_with_label("EDIT");
	StatusButt = gtk_button_new_with_label("Status");
	Notif	= gtk_button_new_with_label("Notifications");
	Upd 	= gtk_button_new_with_label("Updates");

	ChatButt = gtk_button_new();
	ChatLbl  = gtk_label_new("CHAT");
	gtk_widget_modify_font(ChatLbl, df);
	AddButt = gtk_button_new();
	AddLbl  = gtk_label_new("+");
	gtk_widget_modify_font(AddLbl, df);	
	DltButt = gtk_button_new();
	DltLbl  = gtk_label_new("-");
	gtk_widget_modify_font(DltLbl, df);
	
	gtk_container_add(GTK_CONTAINER(AddButt), AddLbl);
	gtk_container_add(GTK_CONTAINER(DltButt), DltLbl);
	gtk_container_add(GTK_CONTAINER(ChatButt), ChatLbl);

	/*create table */
	ButtTable = gtk_table_new(8, 5, TRUE);
	gtk_table_set_homogeneous(GTK_TABLE(ButtTable), TRUE);
	gtk_container_add(GTK_CONTAINER(ButtBox), ButtTable);
	gtk_table_attach_defaults(GTK_TABLE(ButtTable), EditButt, 0, 1, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(ButtTable), StatusButt, 1, 2, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(ButtTable), AddButt, 2, 3, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(ButtTable), DltButt, 3, 4, 0, 1);
	gtk_misc_set_alignment(GTK_MISC(Online), 1, 0);	
	gtk_table_attach_defaults(GTK_TABLE(ButtTable), Online, 0, 2, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(ButtTable), OnlineView, 2, 4, 1, 7);
	gtk_table_attach_defaults(GTK_TABLE(ButtTable), Notif, 0, 2, 7, 8);
	gtk_table_attach_defaults(GTK_TABLE(ButtTable), Upd, 2, 4, 7, 8);
	gtk_table_attach_defaults(GTK_TABLE(ButtTable), vsep, 4, 5, 0, 8);
	
	g_signal_connect(G_OBJECT(EditButt)  , "clicked", G_CALLBACK(editbutton_clicked)  , (gpointer) ListWindow);
	g_signal_connect(G_OBJECT(StatusButt), "clicked", G_CALLBACK(statusbutton_clicked), (gpointer) ListWindow);
	g_signal_connect(G_OBJECT(AddButt)   , "clicked", G_CALLBACK(addbutton_clicked)   , (gpointer) ListWindow);
	g_signal_connect(G_OBJECT(DltButt)   , "clicked", G_CALLBACK(dltbutton_clicked)   , (gpointer) ListWindow);

/* ------------------------------------------------------------------- */
	//tree stuff
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(OnlineView), FALSE);
	label = gtk_label_new(" ");
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(OnlineView));
	init_list(OnlineView);
	
	g_signal_connect(selection, "changed", G_CALLBACK(on_changed), label);	
	
	//pack button box into the hbox
	gtk_box_pack_start_defaults(GTK_BOX(HBox1), ButtBox);
/* ---------------------------------------------------------------- */

	/* chat box start */
	ChatView = gtk_text_view_new();
	ChatScroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(ChatScroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	ChatEntry = gtk_entry_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(ChatView), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(ChatView), FALSE);
	gtk_container_add(GTK_CONTAINER(ChatScroll), ChatView);

	SENDButton = gtk_button_new_with_label("Send");
	
	g_signal_connect(SENDButton, "clicked", G_CALLBACK (SendButton_Clicked), (gpointer) ChatEntry);	
	g_signal_connect (ChatEntry, "changed", G_CALLBACK(UserText), ChatEntry);
	
	ChatTable = gtk_table_new(10, 5, TRUE);
	
	gtk_container_add(GTK_CONTAINER(HBox1), ChatTable);

	gtk_table_attach_defaults(GTK_TABLE(ChatTable), label,   0, 5, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(ChatTable),ChatScroll, 0, 5, 1, 9);
	gtk_table_attach_defaults(GTK_TABLE(ChatTable),ChatEntry, 0, 4, 9, 10);
	gtk_table_attach_defaults(GTK_TABLE(ChatTable),SENDButton, 4, 5, 9, 10);
	
	//gtk_box_pack_start_defaults(GTK_BOX(VBox1), ChatFrame);
	
/* ---------------------------------------------------------------- */

	// Delete Window event
	g_signal_connect(ListWindow, "delete_event", G_CALLBACK(on_delete_event), NULL);
	gtk_widget_set_events(ListWindow, GDK_BUTTON_PRESS_MASK);
	gtk_widget_show_all(ListWindow);

/*	gtk_main(); */
	return ListWindow;
}

// Main LOOP
#ifdef GMAIN
int main( int argc, char *argv[] )
{
	GtkWidget *window;
	window = CreateListWin(&argc, &argv);
	
	gtk_main();
	return 0;
}
#endif

