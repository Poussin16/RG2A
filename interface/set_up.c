#include <gtk/gtk.h>

// ------------ALGO1----------------
int set_algo1[] = {2, 7, 5};

// ------------ALGO1----------------
int set_algo2[] = {4, 5, 10};

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
    const char* label = gtk_button_get_label(start);

    // set to Pause
    if (label[0] == 'S')
    {
        gtk_button_set_label(start, "Pause");
    }
    // set to Start
    else
    {
        gtk_button_set_label(start, "Start");
    }
}

void set_stop(GtkButton* stop, GtkButton* start)
{
    gtk_button_set_label(start, "Start");
}

void set_obstacle(GtkCheckButton* obstacle)
{
    gboolean b = !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obstacle));

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

void get_value_scale(GtkScale* scale, char* s)
{
    g_print("%s: %f\n", s, gtk_range_get_value(GTK_RANGE(scale)));
}

void set_algo(GtkCheckButton* algo, GtkCheckButton* obstacle)
{
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
        }
        else if (algo == algo1_t2)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo2_t2), FALSE);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(path_t2), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(path_t1), FALSE);

            gtk_range_set_value(GTK_RANGE(scale_c2), set_algo1[0]);
            gtk_range_set_value(GTK_RANGE(scale_s2), set_algo1[1]);
            gtk_range_set_value(GTK_RANGE(scale_a2), set_algo1[2]);
        }
        else if (algo == algo2_t1)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo1_t1), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(path_t1),
                !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obstacle)));

            gtk_range_set_value(GTK_RANGE(scale_c1), set_algo2[0]);
            gtk_range_set_value(GTK_RANGE(scale_s1), set_algo2[1]);
            gtk_range_set_value(GTK_RANGE(scale_a1), set_algo2[2]);
        }
        else if (algo == algo2_t2)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo1_t2), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(path_t2),
                !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obstacle)));

            gtk_range_set_value(GTK_RANGE(scale_c2), set_algo2[0]);
            gtk_range_set_value(GTK_RANGE(scale_s2), set_algo2[1]);
            gtk_range_set_value(GTK_RANGE(scale_a2), set_algo2[2]);
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
    }
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
        gtk_widget_set_sensitive(GTK_WIDGET(path_t1), b);

        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo1_t1), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo2_t1), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(path_t1), FALSE);
    }
    else
    {
        gtk_widget_set_sensitive(GTK_WIDGET(scale_c2), b);
        gtk_widget_set_sensitive(GTK_WIDGET(scale_s2), b);
        gtk_widget_set_sensitive(GTK_WIDGET(scale_a2), b);
        gtk_widget_set_sensitive(GTK_WIDGET(algo1_t2), b);
        gtk_widget_set_sensitive(GTK_WIDGET(algo2_t2), b);
        gtk_widget_set_sensitive(GTK_WIDGET(path_t2), b);

        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo1_t2), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(algo2_t2), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(path_t2), FALSE);
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
    scale_c1 = GTK_SCALE(gtk_builder_get_object(builder, "scale_c1"));
    scale_s1 = GTK_SCALE(gtk_builder_get_object(builder, "scale_s1"));
    scale_a1 = GTK_SCALE(gtk_builder_get_object(builder, "scale_a1"));
    algo1_t1 = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "algo1_team1"));
    algo2_t1 = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "algo2_team1"));
    path_t1 = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "path_team1"));
    delete_t1 = GTK_BUTTON(gtk_builder_get_object(builder, "delete_team1"));

    // ------------TEAM2----------------
    scale_c2 = GTK_SCALE(gtk_builder_get_object(builder, "scale_c2"));
    scale_s2 = GTK_SCALE(gtk_builder_get_object(builder, "scale_s2"));
    scale_a2 = GTK_SCALE(gtk_builder_get_object(builder, "scale_a2"));
    algo1_t2 = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "algo1_team2"));
    algo2_t2 = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "algo2_team2"));
    path_t2 = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "path_team2"));
    delete_t2 = GTK_BUTTON(gtk_builder_get_object(builder, "delete_team2"));

    gdouble min = 0;
    gdouble mid = 5;
    gdouble max = 10;
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
    g_signal_connect(scale_c1, "value_changed", G_CALLBACK(get_value_scale), "cohesion 1");
    g_signal_connect(scale_s1, "value_changed", G_CALLBACK(get_value_scale), "separation 1");
    g_signal_connect(scale_a1, "value_changed", G_CALLBACK(get_value_scale), "alignment 1");

    // ------------TEAM2----------------
    g_signal_connect(delete_t2, "clicked", G_CALLBACK(delete_team), NULL);
    g_signal_connect(algo1_t2, "clicked", G_CALLBACK(set_algo), obstacle);
    g_signal_connect(algo2_t2, "clicked", G_CALLBACK(set_algo), obstacle);
    g_signal_connect(scale_c2, "value_changed", G_CALLBACK(get_value_scale), "cohesion 2");
    g_signal_connect(scale_s2, "value_changed", G_CALLBACK(get_value_scale), "separation 2");
    g_signal_connect(scale_a2, "value_changed", G_CALLBACK(get_value_scale), "alignment 2");

    gtk_widget_show(GTK_WIDGET(window_main));
    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
