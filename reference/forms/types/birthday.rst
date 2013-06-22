.. index::
   single: Forms; Fields; birthday

Data urodzin (birthday) - Typ Pola
============================
Pole :doc:`date</reference/forms/types/date>` które specjalizuje się w
obsługiwaniu daty urodzin.

Może zostać wyrenderowany jako pojedyńczy input text, trzy inputy text
(miesiąc, dzień, i rok), lub jako trzy pola typu select.

Typ ten jest zasadniczno identyczny z typem :doc:`date</reference/forms/types/date>`,
ale z bardziej właściwą domyślną wartością dla opcji `years`_. Opcja `years`_
przyjmuje wartość 120 lat wstecz od aktualnego roku.

+------------------------+-----------------------------------------------------------------------------+
| Podstawowe Typy Danych | może być ``DateTime``, ``string``, ``timestamp``, or ``array``              |
|                        | (zobacz :ref:`opcję input<form-reference-date-input>`)                      |
+------------------------+-----------------------------------------------------------------------------+
| Generowane jako        | mogą to być trzy pola select, lub 1 lub 3 pola text, w oparciu              |
|                        | o opcję `widget`_                                                           |
+------------------------+------------------------------------------------------------------------------------------------------------------------+
| Opcje                  | - `years`_                                                                                                             |
+------------------------+------------------------------------------------------------------------------------------------------------------------+
| Odziedziczone          | - `widget`_                                                                                                            |
| opcje                  | - `input`_                                                                                                             |
|                        | - `input`_                                                                    |
|                        | - `months`_                                                                   |
|                        | - `days`_                                                                     |
|                        | - `format`_                                                                   |
|                        | - `data_timezone`_                                                            |
|                        | - `user_timezone`_                                                            |
|                        | - `invalid_message`_                                                          |
|                        | - `invalid_message_parameters`_                                               |
|                        | - `read_only`_                                                                |
|                        | - `disabled`_                                                                 |
|                        | - `mapped`_                                                                   |
|                        | - `inherit_data`_                                                             |
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

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc

Opcje te dziedziczą po typie :doc:`date</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc
