.. index::
   single: Forms; Fields; checkbox

checkbox Typ Pola
=================

Tworzy pojedyńczy input typu checkbox. Powinieneś używać tego zawsze do
pola które posiada wartość typu logicznego (Boolean): jeśli pole jest zaznaczone,
wartość pola zostanie ustawiona na "true", jeśli jest odznaczone, wartość pola
zostanie ustawiona na "false".

+------------------+------------------------------------------------------------------------+
| Renderowane jako | pole ``input`` ``text``                                                |
+------------------+------------------------------------------------------------------------+
| Opcje            | - `value`_                                                             |
+------------------+------------------------------------------------------------------------+
| Odziedziczone    | - `required`_                                                          |
| opcje            | - `label`_                                                             |
|                  | - `read_only`_                                                         |
|                  | - `error_bubbling`_                                                    |
+------------------+------------------------------------------------------------------------+
| Rodzic           | :doc:`field</reference/forms/types/field>`                             |
+------------------+------------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CheckboxType` |
+------------------+------------------------------------------------------------------------+

Przykład Użycia
---------------

.. code-block:: php

    $builder->add('public', 'checkbox', array(
        'label'     => 'Show this entry publicly?',
        'required'  => false,
    ));

Opcje Pola
----------

value
~~~~~

**typ**: ``mixed`` **domyślnie**: ``1``

Wartość która jest użyta jako wartość dla checkboxa. Nie wpływa to
na wartość ustawioną na Twoim obiekcie.

Odziedziczone opcje
-------------------

Opcje te są odziedziczone z typu :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
