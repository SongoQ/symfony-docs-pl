Standardy Kodowania
===================

Gdy wysyłasz kod do Symfony2, musisz stosować się do jego standardów kodowania.
Aby skrócić tę opowieść, mogę powiedzieć Ci złotą zasadę: **Naśladuj istniejący
kod Symfony2**. Większość Bundli open-source oraz bibliotek używanych przez Symfony2
także stosuje się do tych samych wytycznych, Ty także powinieneś.

Pamiętaj że główną zaletą standardów jest to, że każdy kawałek kodu wygląda oraz odbiera
się znajomo, i tu nie chodzi o to, czy to, czy tamto, jest bardziej czytelne.

Jako że zdjęcie - lub trochę kodu - jest wstanie zastąpić tysiące słów, poniżej
jest krótki przykład zawierający większość funkcji opisanych poniżej:

.. code-block:: php

    <?php

    /*
     * This file is part of the Symfony package.
     *
     * (c) Fabien Potencier <fabien@symfony.com>
     *
     * For the full copyright and license information, please view the LICENSE
     * file that was distributed with this source code.
     */

    namespace Acme;

    class Foo
    {
        const SOME_CONST = 42;

        private $foo;

        /**
         * @param string $dummy Some argument description
         */
        public function __construct($dummy)
        {
            $this->foo = $this->transform($dummy);
        }

        /**
         * @param string $dummy Some argument description
         * @return string|null Transformed input
         */
        private function transform($dummy)
        {
            if (true === $dummy) {
                return;
            } elseif ('string' === $dummy) {
                $dummy = substr($dummy, 0, 5);
            }

            return $dummy;
        }
    }

Struktura
---------

* Nie używaj nigdy krótkich tagów (`<?`);

* Nie kończ plików klas tagiem `?>`;

* Wcięcia wykonuj zawsze przy użyciu czterech spacji (tabulacja nie jest dozwolona);

* Używaj znaku końca linii - `0x0A`;

* Używaj jednej spacji po każdym przecinku;

* Nie używaj spacji przed otwarciem oraz zamknięciem nawiasu okrągłego;

* Dodaj pojedyńczą spację wokół operatorów (`==`, `&&`, ...);

* Dodaj pojedyńczą spację przed otwarciem nawiasu okrągłego w strukturach
  kontrolnych (`if`, `else`, `for`, `while`, ...);

* Dodaj pustą linię przed wyrażeniem `return`;

* Nie dodawaj pustych spacji (trailing spaces) na końcu linii;

* Używaj nawiasu klamrowego w celu oznaczenia zawartości struktury kontrolnej
  bez względu na ilość wyrażeń jakie zawiera;

* Wypisuj nawiasy klamrowe w nowej linii dla klas, metod oraz deklaracji
  funkcji;

* Oddziel instrukcje warunkowe (`if`, `else`, ...) oraz otwierający nawias klamrowy
  przy użyciu pojedyńczej spacji a nie nowej pustej linii;

* Deklaruj widoczność w szczególności dla klas, metod, oraz zmiennych klas (używanie
  słowa kluczowego `var` jest zakazane);

* Używaj małych liter dla typów PHP: `false`, `true`, and `null`. To samo tyczy
  się `array()`;

* Używaj dużych liter dla stałych w których słowa oddzielone są znakiem podkreślenia;

* Definiuj jedną klasę na jeden plik;

* Deklaruj zmienne klasy przed metodami;

* Deklaruj najpierw publiczne (public) metody, następnie chronione (protected) a na końcu
  prywatne (private).

Konwencja Nazewnictwa
---------------------

* Używaj camelCase, nie używaj podkreślenia, dla zmiennych, funkcji oraz nazw metod;

* Używaj podkreślenia dla opcji, argumentów, nazw parametrów;

* Używaj przestrzeni nazw dla wszystkich klas;

* Używaj `Symfony` jako przestrzeń nazw pierwszego poziomu;

* Dodawaj przedrostek `Interface` dla interfejsów;

* Używaj alfanumerycznych znaków oraz podkreśleń dla nazw plików;

* Nie zapomnij zapoznać się z bardziej rozbudowanym dokumentem :doc:`conventions`
  dla uzyskania większej ilości informacji w kwestii nazewnictwa.

Dokumentacja
------------

* Dodawaj bloki PHPDoc dla wszystkich klas, metod, oraz funkcji;

* Adnotacje `@package` oraz `@subpackage` nie są używane.

Licencja
--------

* Symfony jest wydany na licencji MIT, informacja o licencji musi być obecna
  na górze każdego pliku PHP, przed przestrzenią nazw.
