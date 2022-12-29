
#include "fpga_config.h"

#include "esp_spiffs.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <sys/stat.h>

#include <array>
#include <iostream>

namespace
{
    constexpr esp_vfs_spiffs_conf_t k_AssetsSpiffsConf =
    {
        .base_path = "/assets",
        .partition_label = "assets",
        .max_files = 4,
        .format_if_mount_failed = false
    };
}  // namespace

extern "C" void app_main()
{
    static constexpr auto k_spinner = std::to_array<char>({'|', '/', '-', '\\'});
    
    std::cout << "Hello world!\n";
    
    auto ret = esp_vfs_spiffs_register(&k_AssetsSpiffsConf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            std::cerr << "Failed to mount or format filesystem\n";
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            std::cerr << "Failed to find SPIFFS partition\n";
        }
        else
        {
            std::cerr << "Failed to initialize SPIFFS (%s)"
                      << esp_err_to_name(ret) << '\n';
        }
        return;
    }
    
/*     std::cout << "Performing SPIFFS_check().\n";
    ret = esp_spiffs_check(k_AssetsSpiffsConf.partition_label);
    if (ret == ESP_OK)
    {
        std::cout << "SPIFFS_check() successful\n";
    }
    else
    {
        std::cerr << "SPIFFS_check() failed!\n";
        return;
    }
    
    size_t total = 0U;
    size_t used = 0U;
    ret = esp_spiffs_info(k_AssetsSpiffsConf.partition_label, &total, &used);
    if (ret == ESP_OK)
    {
        std::cout << "Partition size: total: " << total << ", used: " << used << '\n';
    }
    else
    {
        std::cerr << "Failed to get SPIFFS partition information.\n";
        return;
    }

    struct stat st;
    if (stat("/assets/blinky.bin", &st) == 0)
    {
        std::cout << "File /assets/blinky.bin exists.\n";
    } */
    
    FpgaConfig fpga{{}};
    if (ESP_OK != fpga.configure("/assets/blinky.bin"))
    {
        std::cerr << "Failed to configure FPGA.\n";
    }

    esp_vfs_spiffs_unregister(k_AssetsSpiffsConf.partition_label);
    
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
