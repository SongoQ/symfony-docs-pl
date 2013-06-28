.. index::
   single: formularze; pola; checkbox

Typ pola: checkbox
==================

Tworzy pojedyńcze pole wyboru. Powinno być używane zawsze dla pola, które ma wartość
logiczną – jeśłi pole jest zaznaczona, to zostanie ono ustawione na ``true``,
jeśli nie jest zaznaczone, to wartość zostaje ustawiona na ``false``.

+------------------+------------------------------------------------------------------------+
| Renderowane jako | pole ``input`` ``text``                                                |
+------------------+------------------------------------------------------------------------+
| Opcje            | - `value`_                                                             |
+------------------+------------------------------------------------------------------------+
| Opcje            | - `required`_                                                          |
| odziedziczone    | - `label`_                                                             |
|                  | - `read_only`_                                                         |
|                  | - `error_bubbling`_                                                    |
+------------------+------------------------------------------------------------------------+
| Typ nadrzędny    | :doc:`field</reference/forms/types/field>`                             |
+------------------+------------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CheckboxType` |
+------------------+------------------------------------------------------------------------+

Przykład użycia
---------------

.. code-block:: php
   :linenos:

    $builder->add('public', 'checkbox', array(
        'label'     => 'Show this entry publicly?',
        'required'  => false,
    ));

Opcje pola
----------

value
~~~~~

**typ**: ``mixed`` **domyślnie**: ``1``

Wartość która jest użyta jako wartość dla pola typu *checkbox*. Nie wpływa to na wartość
ustawioną na obiekcie.

Odziedziczone opcje
-------------------

Opcje odziedziczone z typu :doc:`field</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc