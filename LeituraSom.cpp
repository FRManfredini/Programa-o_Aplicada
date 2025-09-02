/**
 * @file LeituraSom.cpp
 * @brief Exemplo de leitura de ADC na placa DK32MP usando interface sysfs
 *
 * Este c�digo implementa uma classe para acessar o conversor anal�gico-digital
 * (ADC) via arquivos do sistema em Linux. Ele permite obter o valor bruto da
 * leitura e convert�-lo para tens�o em Volts.
 *
 * @author
 * D�let, Manfredini e Viegas
 * @date
 * 2025-09-02
 */

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h> // usleep()

 /**
  * @class LeituraSom
  * @brief Classe para leitura de valores do ADC atrav�s da interface sysfs
  *
  * A classe encapsula o acesso a um ADC representado como arquivo no sistema
  * Linux. Fornece m�todos para obter a leitura bruta e convert�-la em tens�o.
  */
class LeituraSom {
private:
    std::string adc_path; ///< Caminho do arquivo do ADC (sysfs)
    float VREF;           ///< Tens�o de refer�ncia (em Volts)
    int RESOLUCAO;        ///< Resolu��o do ADC (m�ximo valor da contagem)
    int leitura;          ///< �ltimo valor lido do ADC

public:
    /**
     * @brief Construtor da classe LeituraSom
     * @param path Caminho do arquivo do ADC no sysfs
     * @param vref Tens�o de refer�ncia (padr�o = 3.3 V)
     * @param resolucao Resolu��o m�xima do ADC (padr�o = 65535)
     */
    LeituraSom(const std::string& path, float vref = 3.3, int resolucao = 65535)
        : adc_path(path), VREF(vref), RESOLUCAO(resolucao), leitura(0) {
    }

    /**
     * @brief L� o valor bruto do ADC a partir do arquivo
     * @return true se a leitura foi realizada com sucesso, false caso contr�rio
     */
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

    /**
     * @brief Converte a �ltima leitura para tens�o (em Volts)
     * @return Valor em Volts correspondente � leitura
     */
    float getTensao() const {
        return leitura * VREF / RESOLUCAO;
    }

    /**
     * @brief Retorna a �ltima leitura bruta do ADC
     * @return Valor inteiro correspondente � leitura
     */
    int getLeitura() const {
        return leitura;
    }
};

/**
 * @brief Fun��o principal
 *
 * Cria um objeto LeituraSom associado ao canal A4 (in_voltage13_raw)
 * e exibe continuamente a leitura bruta e a tens�o correspondente.
 *
 * @return C�digo de sa�da do programa (0 = sucesso)
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