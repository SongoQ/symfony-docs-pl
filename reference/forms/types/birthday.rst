.. index::
   single: formularze; pola; birthday

Typ pola: birthday
==================

Pole :doc:`date</reference/forms/types/date>` które specjalizuje się w
obsługiwaniu daty urodzin.

Może zostać wyrenderowane jako pojedyńcze pole tekstowe, trzy pola tekstowe
(*month*, *day* i *year*) lub jako trzy pola wyboru.

Typ ten jest zasadniczno identyczny z typem :doc:`date</reference/forms/types/date>`,
ale z bardziej właściwą domyślną wartością dla opcji `years`_. Opcja `years`_
przyjmuje wartość 120 lat wstecz od aktualnego roku.

+------------------------+--------------------------------------------------------------------------+
| Podstawowe Typy Danych | może być to ``DateTime``, ``string``, ``timestamp`` lub ``array``        |
|                        | (zobacz :ref:`form-reference-date-input`)                                |
+------------------------+--------------------------------------------------------------------------+
| Generowane jako        | mogą to być trzy pola wyboru albo 1 lub 3 pola tekstowe, oparte na opcji |
|                        | `widget`_                                                                |
+------------------------+--------------------------------------------------------------------------+
| Opcje przesłaniane     | - `years`_                                                               |
+------------------------+--------------------------------------------------------------------------+
| Opcje odziedziczone    | - `widget`_                                                              |
|                        | - `input`_                                                               |
|                        | - `months`_                                                              |
|                        | - `days`_                                                                |
|                        | - `format`_                                                              |
|                        | - `data_timezone`_                                                       |
|                        | - `user_timezone`_                                                       |
|                        | - `invalid_message`_                                                     |
|                        | - `invalid_message_parameters`_                                          |
|                        | - `read_only`_                                                           |
|                        | - `disabled`_                                                            |
|                        | - `mapped`_                                                              |
|                        | - `inherit_data`_                                                        |
+------------------------+--------------------------------------------------------------------------+
| Typ nadrzędny          | :doc:`date</reference/forms/types/date>`                                 |
+------------------------+--------------------------------------------------------------------------+
| Klasa                  | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\BirthdayType`   |
+------------------------+--------------------------------------------------------------------------+

Opcje przesłoniane
------------------

years
~~~~~

**typ**: ``array`` **domyślnie**: 120 lat wstecz od aktualnego roku

Lista lat dostępna typu pola *yaer*. Opcja ta jest istotna tylko gdy opcja ``widget``
jest ustawiona na ``choice``.

Opcje odziedziczone
-------------------

Opcje dziedziczone z typu :doc:`date</reference/forms/types/date>`:

.. include:: /reference/forms/types/options/date_widget.rst.inc
    
.. include:: /reference/forms/types/options/date_input.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/date_format.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc

Opcje dziedziczone z typu :doc:`date</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc
