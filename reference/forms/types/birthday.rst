.. index::
   single: Forms; Fields; birthday

birthday (urodziny) Typ Pola
============================
Pole :doc:`date</reference/forms/types/date>` które specjalizuje się w
obsługiwaniu daty urodzin.

Może zostać wyrenderowany jako pojedyńczy input text, trzy inputy text
(miesiąc, dzień, i rok), lub jako trzy pola typu select.

Typ ten jest zasadniczno identyczny z typem :doc:`date</reference/forms/types/date>`,
ale z bardziej właściwą domyślną wartością dla opcji `years`_. Opcja `years`_
przyjmuje wartość 120 lat wstecz od aktualnego roku.

+------------------------+------------------------------------------------------------------------------------------------------------------------+
| Podstawowe Typy Danych | może być ``DateTime``, ``string``, ``timestamp``, or ``array`` (zobacz :ref:`opcję input<form-reference-date-input>`)  |
+------------------------+------------------------------------------------------------------------------------------------------------------------+
| Renderowane jako       | mogą to być trzy pola select, lub 1 lub 3 pola text, w oparciu o opcję `widget`_                                       |
+------------------------+------------------------------------------------------------------------------------------------------------------------+
| Opcje                  | - `years`_                                                                                                             |
+------------------------+------------------------------------------------------------------------------------------------------------------------+
| Odziedziczone          | - `widget`_                                                                                                            |
| opcje                  | - `input`_                                                                                                             |
|                        | - `months`_                                                                                                            |
|                        | - `days`_                                                                                                              |
|                        | - `format`_                                                                                                            |
|                        | - `pattern`_                                                                                                           |
|                        | - `data_timezone`_                                                                                                     |
|                        | - `user_timezone`_                                                                                                     |
+------------------------+------------------------------------------------------------------------------------------------------------------------+
| Rodzic                 | :doc:`date</reference/forms/types/date>`                                                                               |
+------------------------+------------------------------------------------------------------------------------------------------------------------+
| Klasa                  | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\BirthdayType`                                                 |
+------------------------+------------------------------------------------------------------------------------------------------------------------+

Opcje Pola
---------

years
~~~~~

**typ**: ``array`` **domyślnie**: 120 lat wstecz od aktualnego roku

Lista lat dostępna dla pola typu "year". Opcja ta dotyczy tylko wtedy
gdy opcja ``widget`` jest ustawiona na wartość ``choice``.

Odziedziczone opcje
-----------------

Opcje te są odziedziczone z typu :doc:`date</reference/forms/types/date>`:

.. include:: /reference/forms/types/options/date_widget.rst.inc
    
.. include:: /reference/forms/types/options/date_input.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/date_format.rst.inc
    
.. include:: /reference/forms/types/options/date_pattern.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc
