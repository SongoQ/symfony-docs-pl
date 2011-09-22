.. index::
   single: Forms; Fields; date

date Typ Pola
=============

Pole które umożliwia użytkownikowi modyfikowanie informacji o dacie poprzez
różne elementy HTML.

Podstawowymi danymi przekazanymi do tego typu pola może być obiekt ``DateTime``,
ciąg znaków, timestamp lub tablica. Jak długo opcja `input`_ jest ustawiona
poprawnie, tak długo pole zajmie się całą resztą.

Pole może być wyrenderowane jako pojedyńczy input typu text, trzy inputy typu text
(miesiąc, dzień, oraz rok), lub trzy listy wyboru - select (zobacz opcję `widget_`).

+------------------------+------------------------------------------------------------------------------+
| Podstawowe Typy Danych | może być ``DateTime``, string, timestamp, lub array (zobacz opcję ``input``) |
+------------------------+------------------------------------------------------------------------------+
| Renderowane jako       | pojedyńczy input typu text lub trzy pola typu select                         |
+------------------------+------------------------------------------------------------------------------+
| Opcje                  | - `widget`_                                                                  |
|                        | - `input`_                                                                   |
|                        | - `empty_value`_                                                             |
|                        | - `years`_                                                                   |
|                        | - `months`_                                                                  |
|                        | - `days`_                                                                    |
|                        | - `format`_                                                                  |
|                        | - `pattern`_                                                                 |
|                        | - `data_timezone`_                                                           |
|                        | - `user_timezone`_                                                           |
+------------------------+------------------------------------------------------------------------------+
| Rodzic                 | ``field`` (jeśli tekst), ``form`` w przeciwnym przypadku                     |
+------------------------+------------------------------------------------------------------------------+
| Klasa                  | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\DateType`           |
+------------------------+------------------------------------------------------------------------------+

Podstawowe Użycie
-----------------

Ten typ pola jest wysoce konfigurowalny, lecz prosty w użyciu. Najważniejszymi opcjami są ``input``
oraz ``widget``.

Załóżmy że posiadasz pole ``publishedAt`` którego danymi jest obiekt ``DateTime``. Poniższy kod
konfiguruje typ ``date`` dla tego pola jako trzy różne pola wyboru:

.. code-block:: php

    $builder->add('publishedAt', 'date', array(
        'input'  => 'datetime',
        'widget' => 'choice',
    ));

Opcja ``input`` *musi* zostać zmieniona w stosunku do dostarczonego typu danych.
Dla przykładu, jeśli danymi pola ``publishedAt`` jest unixowy timestamp,
musisz zamienić wartość opcji ``input`` na ``timestamp``:

.. code-block:: php

    $builder->add('publishedAt', 'date', array(
        'input'  => 'timestamp',
        'widget' => 'choice',
    ));

Pole wspiera wartości ``array`` oraz ``string`` dla wartości opcji ``input``.

Opcje Pola
----------

.. include:: /reference/forms/types/options/date_widget.rst.inc

.. _form-reference-date-input:

.. include:: /reference/forms/types/options/date_input.rst.inc

empty_value
~~~~~~~~~~~

**typ**: ``string``|``array``

Jeśli opcja Twojego widgetu jest ustawiona na ``choice``, wtedy pole te będzie reprezentowane
jako lista pól typu ``select``. Opcja ``empty_value`` może zostać użyta do dodania "pustej"
opcji wyboru na górze listy::

    $builder->add('dueDate', 'date', array(
        'empty_value' => '',
    ));

Alternatywnie, możesz zdefiniować ciąg znaków który będzie wyświetlany dla "pustej" wartości::

    $builder->add('dueDate', 'date', array(
        'empty_value' => array('year' => 'Year', 'month' => 'Month', 'day' => 'Day')
    ));

.. include:: /reference/forms/types/options/years.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/date_format.rst.inc

.. include:: /reference/forms/types/options/date_pattern.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc
