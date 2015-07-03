#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include "shared.h"

#include "callbacks.h"
#include "interface.h"
#include "support.h"

//________________________________________________________________________________________________________________________________________
//
// rutine
//_________________________________________________________________________________________________________________________________________
int file_size (gchar *filename)
{
    FILE *fp;
    gint l;

    fp = fopen (filename, "r");
    fseek( fp, 0, 2 );
    l = ftell( fp );
    fclose(fp);
    return l;
}
gchar *load_file (gchar *filename)
{
    FILE *fp;
    gchar *buffer;
    gint l;

    l=file_size(filename);
    fp = fopen (filename, "r");
    buffer = (gchar *) malloc( l );
    fread (buffer, 1,l, fp);
    fclose(fp);
    //g_print("filename=%s size=%d\n",filename,l);
    return buffer;
}

//________________________________________________________________________________________________________________________________________
//
// callback
//_________________________________________________________________________________________________________________________________________


void
on_file1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_load1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    fileselection1 = create_fileselection1();
    gtk_widget_show(fileselection1);


}

void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_print1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_exit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    exit(0);
}


void
on_edit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_cut1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_copy1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_paste1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_find1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_find_next1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_replace1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_select_all1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_invert_selection1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_run1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_execute1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_debug1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_stop1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_item1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_help1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_manual1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_about_ide1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_list1_select_child                  (GtkList         *list,
                                        GtkWidget       *widget,
                                        gpointer         user_data)
{

}


void
on_list2_select_child                  (GtkList         *list,
                                        GtkWidget       *widget,
                                        gpointer         user_data)
{

}


void
on_list3_select_child                  (GtkList         *list,
                                        GtkWidget       *widget,
                                        gpointer         user_data)
{

}


void
on_window1_destroy                     (GtkObject       *object,
                                        gpointer         user_data)
{
    exit(0);
}


void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}

void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
    on_load1_activate(NULL,user_data);
}


void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button6_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button7_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button8_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button9_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}



void
on_ok_button1_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
/*
    filename = gtk_file_selection_get_filename (fileselection1);
    gtk_window_set_title (GTK_WINDOW(window1), filename);
    gtk_text_set_point (GTK_TEXT(text1),0);
    gtk_text_forward_delete (GTK_TEXT(text1),gtk_text_get_length (GTK_TEXT(text1)));
    gtk_text_insert (GTK_TEXT(text1),NULL,NULL,NULL,load_file(filename),file_size(filename));
    gtk_widget_destroy(GTK_WIDGET(fileselection1));
*/
}


void
on_cancel_button1_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{

    gtk_widget_destroy(fileselection1);
}



void
on_close1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_button10_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}

