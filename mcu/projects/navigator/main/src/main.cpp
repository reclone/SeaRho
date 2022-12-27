#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <array>
#include <iostream>

extern "C" void app_main()
{
    static constexpr auto k_spinner = std::to_array<char>({'|', '/', '-', '\\'});
    
    std::cout << "Hello world!\n";
    
    while (true)
    {
        for (auto character : k_spinner)
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            std::cout << character << "\r";
            std::flush(std::cout);
        }
    }
}
