.. index::
   single: Forms; Fields; password

password Typ Pola
=================

Pole ``password`` renderuje input typu password.

+------------------+------------------------------------------------------------------------+
| Renderowane jako | ``input`` ``password`` pole                                            |
+------------------+------------------------------------------------------------------------+
| Opcje            | - `always_empty`_                                                      |
+------------------+------------------------------------------------------------------------+
| Odziedziczone    | - `max_length`_                                                        |
| opcje            | - `required`_                                                          |
|                  | - `label`_                                                             |
|                  | - `trim`_                                                              |
|                  | - `read_only`_                                                         |
|                  | - `error_bubbling`_                                                    |
+------------------+------------------------------------------------------------------------+
| Rodzic           | :doc:`text</reference/forms/types/text>`                               |
+------------------+------------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\PasswordType` |
+------------------+------------------------------------------------------------------------+

Opcje Pola
-------------

always_empty
~~~~~~~~~~~~

**typ**: ``Boolean`` **domyślnie**: ``true``

Jeśli zostanie ustawione na true, pole będzie *zawsze* renderowane puste,
nawet jeśli pole te ma wartość. Jeśli zostanie ustawione na false, pole password
zostanie wyrenderowane z atrybutem ``value`` ustawionym na jego prawdziwą wartość.

Mówiąc prościej, jeśli z jakiś powodów chcesz wyrenderować pole password *z*
już wpisanym hasłem, ustaw tą wartość na false.

Odziedziczone opcje
-------------------

Opcje te są odziedziczone z typu :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
