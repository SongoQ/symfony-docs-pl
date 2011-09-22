.. index::
   single: Forms; Fields; datetime

datetime Typ Pola
=================

Ten typ pola umożliwia użytkownikowi modyfikowanie danych reprezentujących
specyficzną date i czas (np. ``1984-06-05 12:15:30``).

Może być wyrenderowany jako input typu text lub też select. Przekazanymi danymi może być
obiekt ``DateTime``, ciąg znaków, timestamp lub tablica.

+------------------------+------------------------------------------------------------------------------+
| Podstawowe Typy Danych | może być ``DateTime``, string, timestamp, lub array (zobacz opcję ``input``) |
+------------------------+------------------------------------------------------------------------------+
| Renderowane jako       | pojedyńczy input typu text, lub trzy pola typu select                        |
+------------------------+------------------------------------------------------------------------------+
| Opcje                  | - `date_widget`_                                                             |
|                        | - `time_widget`_                                                             |
|                        | - `input`_                                                                   |
|                        | - `date_format`_                                                             |
|                        | - `years`_                                                                   |
|                        | - `months`_                                                                  |
|                        | - `days`_                                                                    |
+------------------------+------------------------------------------------------------------------------+
| Rodzic                 | :doc:`form</reference/forms/types/form>`                                     |
+------------------------+------------------------------------------------------------------------------+
| Klasa                  | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\DateTimeType`       |
+------------------------+------------------------------------------------------------------------------+

Opcje Pola
----------

date_widget
~~~~~~~~~~~

**typ**: ``string`` **domyślnie**: ``choice``

Definiuje opcję ``widget`` dla typu :doc:`date</reference/forms/types/date>`

time_widget
~~~~~~~~~~~

**typ**: ``string`` **domyślnie**: ``choice``

Definiuje opcję ``widget`` dla typu :doc:`time</reference/forms/types/time>`

input
~~~~~

**typ**: ``string`` **domyślnie**: ``datetime``

Format *przekazywanych* danych - np. format danych w jakim jest przechowywany
w Twoim obiekcie. Poprawnymi wartościami są:

* ``string`` (np. ``2011-06-05 12:15:00``)
* ``datetime`` (obiekt ``DateTime``)
* ``array`` (np. ``array(2011, 06, 05, 12, 15, 0)``)
* ``timestamp`` (np. ``1307276100``)

Wartość przekazywana z formularza będzie także normalizowana do zdefiniowanego
formatu.

date_format
~~~~~~~~~~~

**typ**: ``integer`` lub ``string`` **domyślnie**: ``IntlDateFormatter::MEDIUM``

Definiuje opcję ``format`` która będzie przekazana do pola daty.

.. include:: /reference/forms/types/options/hours.rst.inc

.. include:: /reference/forms/types/options/minutes.rst.inc

.. include:: /reference/forms/types/options/seconds.rst.inc

.. include:: /reference/forms/types/options/years.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/with_seconds.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc
