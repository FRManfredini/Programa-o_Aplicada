/**
 * @file LeituraSom.cpp
 * @brief Exemplo de leitura de ADC na placa DK32MP usando interface sysfs
 *
 * Este código implementa uma classe para acessar o conversor analógico-digital
 * (ADC) via arquivos do sistema em Linux. Ele permite obter o valor bruto da
 * leitura e convertê-lo para tensão em Volts.
 *
 * @author
 * Dálet, Manfredini e Viegas
 * @date
 * 2025-09-02
 */

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h> // usleep()

 /**
  * @class LeituraSom
  * @brief Classe para leitura de valores do ADC através da interface sysfs
  *
  * A classe encapsula o acesso a um ADC representado como arquivo no sistema
  * Linux. Fornece métodos para obter a leitura bruta e convertê-la em tensão.
  */
class LeituraSom {
private:
    std::string adc_path; ///< Caminho do arquivo do ADC (sysfs)
    float VREF;           ///< Tensão de referência (em Volts)
    int RESOLUCAO;        ///< Resolução do ADC (máximo valor da contagem)
    int leitura;          ///< Último valor lido do ADC

public:
    /**
     * @brief Construtor da classe LeituraSom
     * @param path Caminho do arquivo do ADC no sysfs
     * @param vref Tensão de referência (padrão = 3.3 V)
     * @param resolucao Resolução máxima do ADC (padrão = 65535)
     */
    LeituraSom(const std::string& path, float vref = 3.3, int resolucao = 65535)
        : adc_path(path), VREF(vref), RESOLUCAO(resolucao), leitura(0) {
    }

    /**
     * @brief Lê o valor bruto do ADC a partir do arquivo
     * @return true se a leitura foi realizada com sucesso, false caso contrário
     */
    bool ler() {
        std::ifstream adc_file(adc_path);
        if (!adc_file.is_open()) {
            std::cerr << "Erro: não consegui abrir " << adc_path << std::endl;
            return false;
        }
        adc_file >> leitura;
        adc_file.close();
        return true;
    }

    /**
     * @brief Converte a última leitura para tensão (em Volts)
     * @return Valor em Volts correspondente à leitura
     */
    float getTensao() const {
        return leitura * VREF / RESOLUCAO;
    }

    /**
     * @brief Retorna a última leitura bruta do ADC
     * @return Valor inteiro correspondente à leitura
     */
    int getLeitura() const {
        return leitura;
    }
};

/**
 * @brief Função principal
 *
 * Cria um objeto LeituraSom associado ao canal A4 (in_voltage13_raw)
 * e exibe continuamente a leitura bruta e a tensão correspondente.
 *
 * @return Código de saída do programa (0 = sucesso)
 */
int main() {
    LeituraSom adcA4("/sys/bus/iio/devices/iio:device0/in_voltage13_raw");

    while (true) {
        if (adcA4.ler()) {
            std::cout << "Leitura ADC: " << adcA4.getLeitura()
                << " | Tensao (V): " << adcA4.getTensao() << std::endl;
        }
        usleep(100000); // 100 ms
    }

    return 0;
}
