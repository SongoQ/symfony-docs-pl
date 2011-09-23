.. index::
   single: Forms; Fields; money

money Typ Pola
==============

Renderuje pole input typu text specjalizujące się w obsłudze danych "pieniężnych".

Pole to pozwala Ci na zdefiniowanie waluty, której symbol jest renderowany obok
pola typu text. Dostępnych jest także kilka opcji dostosowujących w jaki sposób,
wejście jak i wyjście danych zostanie obsłużone.

+------------------+---------------------------------------------------------------------+
| Renderowane jako | ``input`` ``text`` pole                                             |
+------------------+---------------------------------------------------------------------+
| Opcje            | - `currency`_                                                       |
|                  | - `divisor`_                                                        |
|                  | - `precision`_                                                      |
|                  | - `grouping`_                                                       |
+------------------+---------------------------------------------------------------------+
| Odziedziczone    | - `required`_                                                       |
| opcje            | - `label`_                                                          |
|                  | - `read_only`_                                                      |
|                  | - `error_bubbling`_                                                 |
+------------------+---------------------------------------------------------------------+
| Rodzic           | :doc:`field</reference/forms/types/field>`                          |
+------------------+---------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\MoneyType` |
+------------------+---------------------------------------------------------------------+

Opcje Pola
-------------

currency
~~~~~~~~

**typ**: ``string`` **domyślnie**: ``EUR``

Określa walutę w której będą zdefiniowane pieniądze. Określa także symbol waluty
który powinien zostać pokazany obok inputa. W zależności od waluty - symbol waluty
może zostać pokazany przed, lub po, polu tekstowym.

Możliwe jest także ustawienie opcji na false w celu ukrycia symbolu waluty.

divisor
~~~~~~~

**typ**: ``integer`` **domyślnie**: ``1``

Jeśli z jakiegoś powodu, musisz podzielić początkową wartość przez liczbę,
przed jej wyświetleniem użytkownikowi, możesz użyć opcji ``divisor``.
Dla przykładu::

    $builder->add('price', 'money', array(
        'divisor' => 100,
    ));

W tym przypadku, jeśli pole ``price`` jest ustawione na wartość ``9900``, wtedy
użytkownikowi zostanie wyświetlona wartość ``99``. Jeśli użytkownik wyśle wartość
``99``, wtedy wartość ta zostanie pomnożona przez ``100`` i ostatecznie
wartość ``9900`` zostanie zapisana w Twoim obiekcie.

precision
~~~~~~~~~

**typ**: ``integer`` **domyślnie**: ``2``

Jeśli z jakiegoś powodu, potrzebujesz precyzji innej niż 2 miejsca po przecinku,
możesz zmodyfikować tą wartość. Prawdopodobnie nie będziesz miał potrzeby tego robić,
chyba że na przykład, będziesz chciał zaokrąglić do najbliższego dolara (ustaw precyzję na ``0``).

.. include:: /reference/forms/types/options/grouping.rst.inc

Odziedziczone opcje
-------------------

Opcje te są odziedziczone z typu :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
