#include "application.h"

#include "../datasource/csv_file.h"
#include "../datasource/serial_port.h"

Application::Application(int argc, char *argv[])
    : QApplication{argc, argv}
{

}

Application::~Application()
{

}
