/*******Header for Login.c file**********************/

#ifndef LOGIN_H
#define LOGIN_H

/*to let me know when a button is pushed*/
//static void callBack(GtkWidget *widget, gpointer data);
/*setup the exit button signal*/
static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
static void destroy(GtkWidget *widget, gpointer data);


/*"Move" in  between windows window*/
static void LogintoMain(GtkButton *widget, GObject *context_object);

/*get values from login labels*/
static void UserLogin(GtkWidget *widget, GtkWidget *entry);
static void PasswordLogin(GtkWidget *widget, GtkWidget *entry);

/*The Function Call*/
GtkWidget *CreateLoginWindow(int *argc, char **argv[]);

/*moving to register window*/
void MovetoRegWin(int argc, char *argv[]);
#endif
