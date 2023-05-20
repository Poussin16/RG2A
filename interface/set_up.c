#include <err.h>
#include <pthread.h>
#include <gtk/gtk.h>
#include "../Boid/main.h"
#include "../Boid/boid.h"

// -----------THREAD---------------
pthread_t thr;
char status = 0;

// ------------ALGO1----------------
int set_algo1[] = {20, 70, 50};

// ------------ALGO2----------------
int set_algo2[] = {40, 50, 100};

// ------------TEAM1----------------
GtkScale* scale_c1; //cohesion team1
GtkScale* scale_s1; //separation team1
GtkScale* scale_a1; //alignement team1
GtkCheckButton* algo1_t1;
GtkCheckButton* algo2_t1;
GtkCheckButton* path_t1;
GtkButton* delete_t1;

// ------------TEAM2----------------
GtkScale* scale_c2; //cohesion team2
GtkScale* scale_s2; //separation team2
GtkScale* scale_a2; //alignement team2
GtkCheckButton* algo1_t2;
GtkCheckButton* algo2_t2;
GtkCheckButton* path_t2;
GtkButton* delete_t2;


void set_start(GtkButton* start)
{
    // NOT RUN
    // set label PAUSE
    // set run to START
    if (status == 0)
    {
        status = 1;
        set_run(status);
        gtk_button_set_label(start, "Pause");
        int e = pthread_create(&thr, NULL, run, NULL);
        if (e != 0)
            err(EXIT_FAILURE, "pthread create()");
    }
    // RUN
    // set the label to Resume
    // set run to PAUSE
    else if (status == 1)
    {
        status = 2;
        set_run(status);
        gtk_button_set_label(start, "Resume");
    }
    // PAUSE
    // set the label to Pause
    // set run to START
    else if (status == 2)
    {
        status = 1;
        set_run(status);
        gtk_button_set_label(start, "Pause");
    }
}

void set_stop(GtkButton* stop, GtkButton* start)
{
    if (status)
    {
        gtk_button_get_label(stop);
        gtk_button_set_label(start, "Start");
        status = 0;
        set_run(status);
        pthread_join(thr, NULL);
    }
}

void set_obstacle(GtkCheckButton* obstacle)
{
    gboolean b = !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obstacle));

    if (!b)
        set_active_obstacle(1);
    else
        set_active_obstacle(0);

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(path_t1), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(path_t2), FALSE);

    if (b && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(algo2_t1))==TRUE)
        gtk_widget_set_sensitive(GTK_WIDGET(path_t1), TRUE);
    else
        gtk_widget_set_sensitive(GTK_WIDGET(path_t1), FALSE);

    if (b && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(algo2_t2))==TRUE)
        gtk_widget_set_sensitive(GTK_WIDGET(path_t2), TRUE);
    else
        gtk_widget_set_sensitive(GTK_WIDGET(path_t2), FALSE);
}

void set_vector(GtkCheckButton* vector)
{
    gboolean b = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(vector));
    g_print("vector set: %i\n", b);
}

// When a scale was modified
void get_value_scale(GtkScale* scale, int* c)
{
    double coef_s = gtk_range_get_value(GTK_RANGE(scale));
    size_t coef = ((size_t)coef_s * 18) / 10;

    if (*c == 1)
    {
        if (scale == scale_c1)
            set_coef_coh(coef);
        else if (scale == scale_s1)
            set_coef_sep(coef);
        else if (scale == scale_a1)
            set_coef_align(coef);
    }
    else if (*c != 2)
    {
        if (scale == scale_c2)
            set_coef_coh(coef);
        else if (scale ==scale_s2)
            set_coef_sep(coef);
        else if (scale == scale_a2)
            set_coef_align(coef);
    }
}


void set_path(GtkCheckButton* path)
{
    int status_algo = (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(path)) == TRUE) ? 3 : 2;
    set_active_algo(status_algo);
    //g_print("status_algo: %i\n", status_algo);
}
void set_algo(GtkCheckButton* algo, GtkCheckButton* obstacle)
{
    int status_algo = 0;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(algo)) == TRUE)
    {
        if (algo == algo1_t1)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo2_t1), FALSE);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(path_t1), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(path_t1), FALSE);

            gtk_range_set_value(GTK_RANGE(scale_c1), set_algo1[0]);
            gtk_range_set_value(GTK_RANGE(scale_s1), set_algo1[1]);
            gtk_range_set_value(GTK_RANGE(scale_a1), set_algo1[2]);

            status_algo = 1;
        }
        else if (algo == algo1_t2)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo2_t2), FALSE);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(path_t2), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(path_t1), FALSE);

            gtk_range_set_value(GTK_RANGE(scale_c2), set_algo1[0]);
            gtk_range_set_value(GTK_RANGE(scale_s2), set_algo1[1]);
            gtk_range_set_value(GTK_RANGE(scale_a2), set_algo1[2]);

            //status_algo = 1;
        }
        else if (algo == algo2_t1)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo1_t1), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(path_t1),
                !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obstacle)));

            gtk_range_set_value(GTK_RANGE(scale_c1), set_algo2[0]);
            gtk_range_set_value(GTK_RANGE(scale_s1), set_algo2[1]);
            gtk_range_set_value(GTK_RANGE(scale_a1), set_algo2[2]);

            status_algo = (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(path_t1)) == TRUE) ? 3 : 2;
        }
        else if (algo == algo2_t2)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo1_t2), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(path_t2),
                !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obstacle)));

            gtk_range_set_value(GTK_RANGE(scale_c2), set_algo2[0]);
            gtk_range_set_value(GTK_RANGE(scale_s2), set_algo2[1]);
            gtk_range_set_value(GTK_RANGE(scale_a2), set_algo2[2]);

            //status_algo = 2
        }
    }
    else
    {
        if (algo == algo2_t1)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(path_t1), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(path_t1), FALSE);
        }
        else if (algo == algo2_t2)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(path_t2), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(path_t2), FALSE);
        }
        status_algo = 0;
    }

    //g_print("status_algo: %i\n", status_algo);
    set_active_algo(status_algo);
}

void delete_team(GtkButton* delete)
{
    gboolean b;
    const char* label = gtk_button_get_label(delete);
    if (label[0] == 'D')
    {
        gtk_button_set_label(delete, "Add");
        b = FALSE;
    }
    else
    {
        gtk_button_set_label(delete, "Delete");
        b = TRUE;
    }

    if (delete == delete_t1)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(scale_c1), b);
        gtk_widget_set_sensitive(GTK_WIDGET(scale_s1), b);
        gtk_widget_set_sensitive(GTK_WIDGET(scale_a1), b);
        gtk_widget_set_sensitive(GTK_WIDGET(algo1_t1), b);
        gtk_widget_set_sensitive(GTK_WIDGET(algo2_t1), b);

        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo1_t1), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo2_t1), FALSE);
    }
    else
    {
        gtk_widget_set_sensitive(GTK_WIDGET(scale_c2), b);
        gtk_widget_set_sensitive(GTK_WIDGET(scale_s2), b);
        gtk_widget_set_sensitive(GTK_WIDGET(scale_a2), b);
        gtk_widget_set_sensitive(GTK_WIDGET(algo1_t2), b);
        gtk_widget_set_sensitive(GTK_WIDGET(algo2_t2), b);

        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo1_t2), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo2_t2), FALSE);
    }
}

int main ()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow* window_main = GTK_WINDOW(gtk_builder_get_object(builder, "window_main"));
    GtkButton* start = GTK_BUTTON(gtk_builder_get_object(builder, "button_start"));
    GtkButton* stop = GTK_BUTTON(gtk_builder_get_object(builder, "button_stop"));
    GtkCheckButton* obstacle = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "obstacle"));
    GtkCheckButton* vector = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "vector"));

    // ------------TEAM1----------------
    int team1 = 1;
    scale_c1 = GTK_SCALE(gtk_builder_get_object(builder, "scale_c1"));
    scale_s1 = GTK_SCALE(gtk_builder_get_object(builder, "scale_s1"));
    scale_a1 = GTK_SCALE(gtk_builder_get_object(builder, "scale_a1"));
    algo1_t1 = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "algo1_team1"));
    algo2_t1 = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "algo2_team1"));
    path_t1 = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "path_team1"));
    delete_t1 = GTK_BUTTON(gtk_builder_get_object(builder, "delete_team1"));

    // ------------TEAM2----------------
    int team2 = 2;
    scale_c2 = GTK_SCALE(gtk_builder_get_object(builder, "scale_c2"));
    scale_s2 = GTK_SCALE(gtk_builder_get_object(builder, "scale_s2"));
    scale_a2 = GTK_SCALE(gtk_builder_get_object(builder, "scale_a2"));
    algo1_t2 = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "algo1_team2"));
    algo2_t2 = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "algo2_team2"));
    path_t2 = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "path_team2"));
    delete_t2 = GTK_BUTTON(gtk_builder_get_object(builder, "delete_team2"));

    gdouble min = 0;
    gdouble mid = 50;
    gdouble max = 100;
    gtk_range_set_range(GTK_RANGE(scale_c1), min, max);
    gtk_range_set_value(GTK_RANGE(scale_c1), mid);
    gtk_range_set_range(GTK_RANGE(scale_s1), min, max);
    gtk_range_set_value(GTK_RANGE(scale_s1), mid);
    gtk_range_set_range(GTK_RANGE(scale_a1), min, max);
    gtk_range_set_value(GTK_RANGE(scale_a1), mid);

    gtk_range_set_range(GTK_RANGE(scale_c2), min, max);
    gtk_range_set_value(GTK_RANGE(scale_c2), mid);
    gtk_range_set_range(GTK_RANGE(scale_s2), min, max);
    gtk_range_set_value(GTK_RANGE(scale_s2), mid);
    gtk_range_set_range(GTK_RANGE(scale_a2), min, max);
    gtk_range_set_value(GTK_RANGE(scale_a2), mid);

    // Connects signal handlers.
    g_signal_connect(window_main, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(start, "clicked", G_CALLBACK(set_start), NULL);
    g_signal_connect(stop, "clicked", G_CALLBACK(set_stop), start);
    g_signal_connect(obstacle, "toggled", G_CALLBACK(set_obstacle), NULL);
    g_signal_connect(vector, "toggled", G_CALLBACK(set_vector), NULL);

    // ------------TEAM1----------------
    g_signal_connect(delete_t1, "clicked", G_CALLBACK(delete_team), NULL);
    g_signal_connect(algo1_t1, "clicked", G_CALLBACK(set_algo), obstacle);
    g_signal_connect(algo2_t1, "clicked", G_CALLBACK(set_algo), obstacle);
    g_signal_connect(path_t1, "clicked", G_CALLBACK(set_path), NULL);
    g_signal_connect(scale_c1, "value_changed", G_CALLBACK(get_value_scale), &team1);
    g_signal_connect(scale_s1, "value_changed", G_CALLBACK(get_value_scale), &team1);
    g_signal_connect(scale_a1, "value_changed", G_CALLBACK(get_value_scale), &team1);

    // ------------TEAM2----------------
    g_signal_connect(delete_t2, "clicked", G_CALLBACK(delete_team), NULL);
    g_signal_connect(algo1_t2, "clicked", G_CALLBACK(set_algo), obstacle);
    g_signal_connect(algo2_t2, "clicked", G_CALLBACK(set_algo), obstacle);
    g_signal_connect(path_t2, "clicked", G_CALLBACK(set_path), NULL);
    g_signal_connect(scale_c2, "value_changed", G_CALLBACK(get_value_scale), &team2);
    g_signal_connect(scale_s2, "value_changed", G_CALLBACK(get_value_scale), &team2);
    g_signal_connect(scale_a2, "value_changed", G_CALLBACK(get_value_scale), &team2);

    gtk_widget_show(GTK_WIDGET(window_main));
    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
