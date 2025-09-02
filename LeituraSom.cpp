#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h> // usleep()

class ADC {
private:
    std::string adc_path; // caminho do arquivo do ADC
    float VREF;           // tens�o de refer�ncia
    int RESOLUCAO;        // resolu��o do ADC
    int leitura;          // �ltimo valor lido

public:
    // Construtor
    ADC(const std::string& path, float vref = 3.3, int resolucao = 65535)
        : adc_path(path), VREF(vref), RESOLUCAO(resolucao), leitura(0) {
    }

    // L� o valor bruto do ADC
    bool ler() {
        std::ifstream adc_file(adc_path);
        if (!adc_file.is_open()) {
            std::cerr << "Erro: n�o consegui abrir " << adc_path << std::endl;
            return false;
        }
        adc_file >> leitura;
        adc_file.close();
        return true;
    }

    // Converte a leitura para tens�o
    float getTensao() const {
        return leitura * VREF / RESOLUCAO;
    }

    // Retorna a leitura bruta
    int getLeitura() const {
        return leitura;
    }
};

int main() {
    ADC adcA4("/sys/bus/iio/devices/iio:device0/in_voltage13_raw");

    while (true) {
        if (adcA4.ler()) {
            std::cout << "Leitura ADC: " << adcA4.getLeitura()
                << " | Tensao (V): " << adcA4.getTensao() << std::endl;
        }
        usleep(100000); // 100 ms
    }

    return 0;
}
