.. index::
   single: YAML
   single: Configuration; YAML

YAML
====

Strona `YAML`_ opisuje ten format jako "przyjazny dla człowieka standard serializacji
danych dla wszystkich języków programowania". YAML jest prostym językiem do opisywania
danych. Tak jak PHP, YAML posiada prostą składnię dla takich typów jak string, boolean,
float, czy też integer. Ale w przeciwieństwie do PHP, YAML rozróżnia tablice sekwencyjne
(sequences) oraz mapowane (mappings).

Komponent :namespace:`Symfony\\Component\\Yaml` z Symfony2 radzi sobie z parsowaniem YAMLa
oraz potrafi tworzyć tablice YAML z tablic PHP.

.. note::

    Format YAML potrafi opisywać skomplikowane zagnieżdżone struktury danych,
    ale rozdział ten opisuje tylko minimalną część funkcjonalności potrzebnej
    do używania YAML jako formatu zapisu plików konfiguracyjnych.

Czytanie Plików YAML
--------------------
Metoda :method:`Symfony\\Component\\Yaml\\Parser::parse` parsuje ciąg znaków 
YAML oraz konwertuje go do tablicy PHP::

    use Symfony\Component\Yaml\Parser;

    $yaml = new Parser();
    $value = $yaml->parse(file_get_contents('/path/to/file.yaml'));

Jeśli podczas parsowania wystąpi błąd, parser wyrzuci wyjątek wskazując rodzaj
błędu oraz linie w oryginalnym ciągu znaków YAML gdzie błąd wystąpił::

    try {
        $value = $yaml->parse(file_get_contents('/path/to/file.yaml'));
    } catch (\InvalidArgumentException $e) {
        // an error occurred during parsing
        echo "Unable to parse the YAML string: ".$e->getMessage();
    }

.. tip::

    Jeśli obiekt parsera jest zdefiniowany, możesz użyć go do ładowania
    różnych ciągów YAML.

Gdy chcesz załadować plik YAML, czasami lepiej jest użyć metody
:method:`Symfony\\Component\\Yaml\\Yaml::parse`::

    use Symfony\Component\Yaml\Yaml;

    $loader = Yaml::parse('/path/to/file.yml');

Metoda statyczna ``Yaml::parse()`` jako argument przyjmuje ciąg YAML lub też
plik zawierający YAML. Wewnętrznie wywoływana jest metoda ``Parser::parse()``,
ale z kilkoma dodatkami:

* Wykonuje plik YAML traktując go jako plik PHP, dzięki temu możesz osadzić 
  komendy PHP w pliku YAML;

* Jeśli plik nie może zostać sparsowany, automatycznie zostanie dodana jego nazwa
  do wiadomości błędu, ułatwiając debugowanie gdy aplikacja ładuje kilka plików YAML.


Zapisywanie Plików YAML
-----------------------

Metoda :method:`Symfony\\Component\\Yaml\\Dumper::dump` zrzuca każdą tablicę PHP
do formatu YAML::

    use Symfony\Component\Yaml\Dumper;

    $array = array('foo' => 'bar', 'bar' => array('foo' => 'bar', 'bar' => 'baz'));

    $dumper = new Dumper();
    $yaml = $dumper->dump($array);
    file_put_contents('/path/to/file.yaml', $yaml);

.. note::

   Metoda posiada kilka ograniczeń: nie ma możliwości zrzucania zasobów,
   oraz funkcjonalność zrzucania obiektów PHP jest w fazie alpha.

Jeśli chcesz zrzucić tylko jedną tablicę, możesz użyć krótszej statycznej metody
:method:`Symfony\\Component\\Yaml\\Yaml::dump`::

    $yaml = Yaml::dump($array, $inline);

Format YAML obsługuje dwie reprezentacje tablic YAML. Domyślnie, zrzucana tablica
przedstawiana jest w jednej linii:

.. code-block:: yaml

    { foo: bar, bar: { foo: bar, bar: baz } }

Ale drugi argument metody ``dump()`` dostosowuje poziom na którym z widoku
rozszerzonego zrzutu metoda wraca do widoku w jednej linii:

    echo $dumper->dump($array, 1);

.. code-block:: yaml

    foo: bar
    bar: { foo: bar, bar: baz }

.. code-block:: php

    echo $dumper->dump($array, 2);

.. code-block:: yaml

    foo: bar
    bar:
        foo: bar
        bar: baz

Składnia YAML
-------------

Ciągi Znaków (Strings)
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: yaml

    Ciąg znaków w YAML

.. code-block:: yaml

    'Ciąg znaków YAML w pojedyńczym cudzysłowiu'

.. tip::
   W ciągu znaków pomiędzy pojedyńczymi cudzysłowiami, pojedyńczy cudzysłów ``'``
   musi zostać powtórzony:

   .. code-block:: yaml

        'Pojedyńczy cudzysłów '' w ciągu znaków pomiędzy pojedyńczymi cudzysłowami'

.. code-block:: yaml

    "Ciąg znaków YAML w podwójnym cudzysłowiu\n"

Stosowanie cudzysłowów jest użyteczne gdy ciąg znaków zaczyna się lub kończy, jednym
lub też kilkoma spacjami.

.. tip::

    Podwójne cudzysłowia pozwalają na stosowanie znaków specjalnych, poprzez dodanie
    znaku ``\``. Jest to bardzo pomocne gdy chcesz osadzic ``\n`` lub też znak Unicode
    w ciągu.

Jeśli ciąg znaków zawiera nowe linie, możesz użyć znaku ``|`` który określał będzie że 
ciąg będzie znajdował się w kilku liniach:

.. code-block:: yaml

    |
      \/ /| |\/| |
      / / | |  | |__

Alternatywnym sposobem, jest stosowanie stylu "składanego", oznaczonego przez ``>``,
w stylu tym każda nowa linia zostanie zastąpiona spacją:

    >
      This is a very long sentence
      that spans several lines in the YAML
      but which will be rendered as a string
      without carriage returns.

.. note::

    Zwróć uwagę na dwie spacje przed każdą linią w poprzednim przykładzie. Nie pojawią
    się w wyniku PHP.

Liczby
~~~~~~

.. code-block:: yaml

    # liczba
    12

.. code-block:: yaml

    # zapis ósemkowy
    014

.. code-block:: yaml

    # zapis szesnastkowy
    0xC

.. code-block:: yaml

    # liczba zmiennoprzecinkowa
    13.4

.. code-block:: yaml

    # liczba wykładnicza
    1.2e+34

.. code-block:: yaml

    # nieskończoność
    .inf

NULLe
~~~~~

Nulle w YAML mogą być wyrażone poprzez ``null`` lub ``~``.

Wartości logiczne (Booleans)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Wartości logiczne w YAML mogą być wyrażone przez ``true`` oraz ``false``.

Daty
~~~~

YAML używa standardu ISO-8601 do reprezentowania daty:

.. code-block:: yaml

    2001-12-14t21:59:43.10-05:00

.. code-block:: yaml

    # prosta data
    2002-12-14

Kolekcje
~~~~~~~~

Plik YAML jest rzadko stosowany do opisywania prostych danych skalarnych. W większości
przypadków opisywane są kolekcje. Kolekcją może być sekwencja (sequence) lub też 
mapowanie (mapping) reprezentacja elementów. Obydwa są konwertowane do tablic PHP.

Sekwencja stosuje kreski po której występuje spacja (``-`` ):

.. code-block:: yaml

    - PHP
    - Perl
    - Python

Poprzedni YAML jest równoznaczny z zapisem PHP::

    array('PHP', 'Perl', 'Python');

Mapowanie używa dwukropku po którym występuje spacja (``:`` ) aby zaznaczyć każdą
parę klucz/wartość:

.. code-block:: yaml

    PHP: 5.2
    MySQL: 5.1
    Apache: 2.2.20

który to posiada odpowiednik w PHP::

    array('PHP' => 5.2, 'MySQL' => 5.1, 'Apache' => '2.2.20');

.. note::

   W mapowaniu, kluczem może być każda poprawna wartość skalarna.

Ilość spacji pomiędzy dwukropkiem a wartością nie ma znaczenia:

.. code-block:: yaml

    PHP:    5.2
    MySQL:  5.1
    Apache: 2.2.20

YAML wykorzystuje wcięcia w postaci spacji, do określenia zagnieżdżenia kolekcji:

.. code-block:: yaml

    "symfony 1.4":
        PHP:      5.2
        Doctrine: 1.2
    "Symfony2":
        PHP:      5.3
        Doctrine: 2.0

Powyższy kod YAML jest równoznaczny z kodem PHP::

    array(
        'symfony 1.4' => array(
            'PHP'      => 5.2,
            'Doctrine' => 1.2,
        ),
        'Symfony2' => array(
            'PHP'      => 5.3,
            'Doctrine' => 2.0,
        ),
    );

Jest jedna bardzo ważna rzecz o której musisz pamiętać stosując wcięcia w pliku YAML:
*Wcięcia muszą zostać zrobione z jednej lub większej ilości spacji, nigdy przy użyciu
tabulatora*.

Możesz grupować sekwencje oraz mapowania jak tylko chcesz:

.. code-block:: yaml

    'Chapter 1':
        - Introduction
        - Event Types
    'Chapter 2':
        - Introduction
        - Helpers

YAML może także stosować styl przepływu, wykorzystując wyraźnych wskaźników
zamiast wcięć do określenia zakresu.

Sekwencja może zostać zapisana jako lista rozdzielona przecinkami wewnątrz
nawiasów kwadratowych (``[]``):

.. code-block:: yaml

    [PHP, Perl, Python]

Mapowanie może zostać zapisane jako lista kluczy/wartości rozdzielonych
przecinkami pomiędzy nawiasami klamrowymi (``{}``):

.. code-block:: yaml

    { PHP: 5.2, MySQL: 5.1, Apache: 2.2.20 }

Możesz mieszać z sobą style do osiągnięcia lepszej czytelności:

.. code-block:: yaml

    'Chapter 1': [Introduction, Event Types]
    'Chapter 2': [Introduction, Helpers]

.. code-block:: yaml

    "symfony 1.4": { PHP: 5.2, Doctrine: 1.2 }
    "Symfony2":    { PHP: 5.3, Doctrine: 2.0 }

Komentarze:
~~~~~~~~~~~

Komentarze mogą być dodawane do YAML poprzez poprzedzenie je znakiem ``#``:

.. code-block:: yaml

    # Komentarz na całą linię
    "Symfony2": { PHP: 5.3, Doctrine: 2.0 } # Komentarz na końcu linii

.. note::

    Komentarze są po prostu ignorowane przez parser YAML i nie muszą być
    wcięte w stosunku do aktualnego zagnieżdżenia w kolekcji.

Dynamiczne pliki YAML
~~~~~~~~~~~~~~~~~~~~~

W Symfony2, pliki YAML mogą zawierać kod PHP który wykonywany tuż przed
wykonaniem parsowania:

.. code-block:: yaml

    1.0:
        version: <?php echo file_get_contents('1.0/VERSION')."\n" ?>
    1.1:
        version: "<?php echo file_get_contents('1.1/VERSION') ?>"

Uważaj aby nie nabałaganić z wcięciami. Należy pamiętać o kilku prostych
wskazówkach gdy dodajesz kod PHP do pliku YAML:

* Deklaracja ``<?php ?>`` musi zawsze być wywoływana na początku lini lub też
  być osadzona jako wartość.

* Jeśli deklaracja ``<?php ?>`` kończy linię, musisz pamiętać o dodaniu nowej
  linii ("\n").

.. _YAML: http://yaml.org/
