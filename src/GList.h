/*************************************************
 * GList.h
 * Header file for Main List GUI, GList.c
 *
 * Author  : Bianca Doronila
 * Created : 03/09/2018
 *
 * Modifications:
 *
 *
 *
 * **********************************************/

#ifndef GLIST_H
#define GLIST_H

/* add to tree list -------- this function adds friends into the txt file */
void add_to_list(GtkWidget *list, const gchar *str);

/* initialize your tree list ------ this function initializes user's friendslist if exists already */
void init_list(GtkWidget *list);

//void on_changed(GtkWidget *widget, gpointer label);

static void editbutton_clicked(GtkButton *EditButt, GtkWindow *ListWindow);
static void addbutton_clicked(GtkButton *AddButt, GtkWindow *ListWindow);
static void dltbutton_clicked(GtkButton *DltButt, GtkWindow *ListWindow);
static void statusbutton_clicked(GtkButton *StatusButt, GtkWindow *ListWindow);

static gboolean on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

static void UserText(GtkWidget *widget, GtkEntry *entry);

static void SendButton_Clicked(GtkWidget *widget, gpointer data);

static gboolean ServerUpdate(gpointer data);

void ServerRun();

GtkWidget *CreateListWin(int *argc, char **argv[]);

#endif
