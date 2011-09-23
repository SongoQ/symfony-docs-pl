.. index::
   single: Forms; Fields; integer

integer Typ Pola
================

Renderuje "numeryczne" pole typu input. Zasadniczo, jest to pole tekstowe które
jest dobre do obsługi liczb całkowitych w formularzu. Input ``number`` wygląda
identycznie jak input text, z wyjątkiem - jeśli przeglądarka użytkownika wspiera HTML5
- pole będzie posiadało kilka dodatkowych funkcjonalności.

Pole te posiada kilka różnych opcji do obsługi wartości które nie są liczbami stałymi.
Domyślnie, wszystkie wartości nie będące liczbami stałymi (np. 6,78) zostaną zaokrąglone
w dół (np. 6).

+------------------+-----------------------------------------------------------------------+
| Renderowane jako | ``input`` ``text`` pole                                               |
+------------------+-----------------------------------------------------------------------+
| Opcje            | - `rounding_mode`_                                                    |
|                  | - `grouping`_                                                         |
+------------------+-----------------------------------------------------------------------+
| Odziedziczone    | - `required`_                                                         |
| opcje            | - `label`_                                                            |
|                  | - `read_only`_                                                        |
|                  | - `error_bubbling`_                                                   |
+------------------+-----------------------------------------------------------------------+
| Rodzic           | :doc:`field</reference/forms/types/field>`                            |
+------------------+-----------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\IntegerType` |
+------------------+-----------------------------------------------------------------------+

Opcje Pola
----------

rounding_mode
~~~~~~~~~~~~~

**typ**: ``integer`` **domyślnie**: ``IntegerToLocalizedStringTransformer::ROUND_DOWN``

Domyślnie, jeśli użytkownik wprowadzi wartość nie będącą liczbą całkowitą, zostanie ona
zaokrąglona w dół. Jest kilka sposobów zaokrąglania, każda z nich posiada stałą w klasie
:class:`Symfony\\Component\\Form\\Extension\\Core\\DataTransformer\\IntegerToLocalizedStringTransformer`:

*   ``IntegerToLocalizedStringTransformer::ROUND_DOWN`` Tryb zaokrąglania,
    zaokrąglający do zera.

*   ``IntegerToLocalizedStringTransformer::ROUND_FLOOR`` Tryb zaokrąglania,
    zaokrąglający do minus nieskończoności.

*   ``IntegerToLocalizedStringTransformer::ROUND_UP`` Tryb zaokrąglania,
    zaokrąglający od zera.

*   ``IntegerToLocalizedStringTransformer::ROUND_CEILING`` Tryb zaokrąglania,
    zaokrągla do plus nieskończoności.

.. include:: /reference/forms/types/options/grouping.rst.inc

Odziedziczone opcje
-------------------

Opcje te są odziedziczone z typu :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
