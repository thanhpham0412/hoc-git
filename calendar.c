#include <gtk/gtk.h>
#include "makedate.c"

int currentMonth;
int currentYear;
GtkWidget *subGrid;
GtkWidget *grid;
GtkWidget *yearButton;
GtkWidget *monthButton;

static void makeYearButton()
{
    gtk_widget_destroy(yearButton);
    char strYear[10];
	sprintf(strYear, "%d", currentYear);
    yearButton = gtk_button_new_with_label(strYear);
    gtk_grid_attach(GTK_GRID(grid), yearButton, 1, 0, 1, 1);
    gtk_widget_show_all(GTK_WIDGET(yearButton));
}

static void makeCalendar()
{
    gtk_widget_destroy(monthButton);
    monthButton = gtk_button_new_with_label(months[currentMonth - 1]);
    gtk_grid_attach(GTK_GRID(grid), monthButton, 1, 1, 1, 1);

    gtk_widget_destroy(subGrid);
    subGrid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), subGrid, 0, 2, 3, 1);

    getVal(currentYear);

	GtkWidget *button;

    //create days in week
    for (int i = 1; i <= 7; i++) {
        button = gtk_button_new_with_label(days[i - 1]);
        gtk_grid_attach(GTK_GRID(subGrid), button, i, 0, 1, 1);
    }

    int numEmptyDay = emptyDay[currentMonth - 1];
	int numDay = 1;
	int postDay = 1;
	int maxColumns = 5;
	if (numEmptyDay + maxDay[currentMonth - 1] > 35)
        maxColumns = 6;
    //make week button
    int numWeek = numOfWeek[currentMonth - 1];
    for (int i = 1; i <= maxColumns; i++) {
        char strWeek[10];
        char str[] = "WEEK ";
        sprintf(strWeek, "%d", numWeek);
        if (numWeek != 0)
            strcat(str, strWeek);
        button = gtk_button_new_with_label(str);
        if (numWeek == 0)
            gtk_widget_set_opacity(button, 0.5);
        gtk_grid_attach(GTK_GRID(subGrid), button, 0, i, 1, 1);
        numWeek++;
    }
    //make day button
    for (int i = 1; i <= maxColumns; i++)
        for (int j = 1; j <= 7; j++)
        {
            if (numEmptyDay != 0)
            {
                if (currentMonth > 1)
                {
                    button = gtk_button_new_with_label(date[maxDay[currentMonth - 2] - numEmptyDay + 1]);
                } else
                    button = gtk_button_new_with_label(date[31 - numEmptyDay + 1]);
                gtk_widget_set_opacity(button, 0.5);
                numEmptyDay--;
            } else if (numDay <= maxDay[currentMonth - 1])
            {
                button = gtk_button_new_with_label(date[numDay]);
                //g_signal_connect(button, "clicked", G_CALLBACK(makeNote), NULL);
                numDay++;
            } else {
                button = gtk_button_new_with_label(date[postDay]);
                gtk_widget_set_opacity(button, 0.5);
                postDay++;
            }
            gtk_grid_attach(GTK_GRID(subGrid), button, j, i, 1, 1);
        }
    gtk_widget_show_all(GTK_WIDGET(grid));
}
static void downYear()
{
        currentYear--;
        makeYearButton();
        makeCalendar();
}
static void upYear()
{
        currentYear++;
        makeYearButton();
        makeCalendar();
}

static void downMonth()
{
    if (currentMonth > 1) {
        currentMonth--;
        makeCalendar();
    } else {
        currentMonth = 12;
        currentYear--;
        makeYearButton();
        makeCalendar();
    }
}
static void upMonth()
{
    if (currentMonth < 12) {
        currentMonth++;
        makeCalendar();
    } else {
        currentMonth = 1;
        currentYear++;
        makeYearButton();
        makeCalendar();
    }
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *button;
    //create window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Calendar");
    //gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
    //create grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    //change year button
    button = gtk_button_new_with_label("<<");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(downYear), NULL);
    button = gtk_button_new_with_label(">>");
    gtk_grid_attach(GTK_GRID(grid), button, 2, 0, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(upYear), NULL);
    //change month button
    button = gtk_button_new_with_label("<");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(downMonth), NULL);
    button = gtk_button_new_with_label(">");
    gtk_grid_attach(GTK_GRID(grid), button, 2, 1, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(upMonth), NULL);
    //quit button
    button = gtk_button_new_with_label("Quit");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 3, 3, 1);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);

    //initialize
    currentYear = 2021;
    currentMonth = 5;
    subGrid = gtk_grid_new();
    char strYear[10];
    sprintf(strYear, "%d", currentYear);
    yearButton = gtk_button_new_with_label(strYear);
    gtk_grid_attach(GTK_GRID(grid), yearButton, 1, 0, 1, 1);

    monthButton = gtk_button_new_with_label(months[currentMonth - 1]);
    gtk_grid_attach(GTK_GRID(grid), monthButton, 1, 1, 1, 1);

    makeCalendar();

    gtk_widget_show_all(window);
}
int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("calendar.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
