.. index::
   single: formularze; pola; choice

Typ pola: choice
================

Wielofunkcyjne pole używane do umożliwienia użytkownikowi wybrania jednej lub kilku
opcji. Może być wyrenderowany jako zacznik ``select``, przyciski radio lub pola
wyboru.

Aby użyć to pole, musi się podać opcję ``choice_list`` albo ``choices``.

+------------------+----------------------------------------------------------------------+
| Rednerowane jako | może to być kilka znaczników (zobacz poniżej)                        |
+------------------+----------------------------------------------------------------------+
| Opcje            | - `choices`_                                                         |
|                  | - `choice_list`_                                                     |
|                  | - `multiple`_                                                        |
|                  | - `expanded`_                                                        |
|                  | - `preferred_choices`_                                               |
|                  | - `empty_value`_                                                     |
+------------------+----------------------------------------------------------------------+
| Opcje            | - `required`_                                                        |
| odziedziczone    | - `label`_                                                           |
|                  | - `read_only`_                                                       |
|                  | - `disabled`_                                                        |
|                  | - `error_bubbling`_                                                  |
|                  | - `mapped`_                                                          |
|                  | - `inherit_data`_                                                    |
|                  | - `by_reference`_                                                    |
|                  | - `empty_data`_                                                      |
+------------------+----------------------------------------------------------------------+
| Typ nadrzędny    | :doc:`form</reference/forms/types/form>` (jeśli rozszerzone),        |
|                  | inaczej ``field``                                                    |
+------------------+----------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\ChoiceType` |
+------------------+----------------------------------------------------------------------+

Przykład użycia
---------------

Najprostszym sposobem na użycie tego pola jest bezpośrenie określenie pól wyboru
poprzez opcję ``choices``. Klucz elementu tablicy staje się wartością, która w
rzeczywistości ustawia się w odnośnym obiekcie (np. ``m``), podczas gdy wartość
jest tym, co użytkownik widzi w formularzu (np. ``Male``).

.. code-block:: php
   :linenos:

    $builder->add('gender', 'choice', array(
        'choices'   => array('m' => 'Male', 'f' => 'Female'),
        'required'  => false,
    ));

Ustawiając opcję ``multiple`` na true, umożliwia sie użytkownikowi wybór
wielu  wartości. Widget będzie wyrenderowany jako złozony znacznik``select`` lub
jako seria pól wyboru w zalezności od opcji ``expanded``:

.. code-block:: php
   :linenos:

    $builder->add('availability', 'choice', array(
        'choices'   => array(
            'morning'   => 'Morning',
            'afternoon' => 'Afternoon',
            'evening'   => 'Evening',
        ),
        'multiple'  => true,
    ));

Można również użyć opcję ``choice_list``, która pobiera obiekt mogący określić
pola wyboru dla widgetu.

.. _forms-reference-choice-tags:

.. include:: /reference/forms/types/options/select_how_rendered.rst.inc

Opcje pola
----------

choices
~~~~~~~

**typ**: ``array`` **domyślnie**: ``array()``

Jest to najbardziej podstawowy sposób określenia pól typu *choice*,
które powinny być użyte przez to pole. Opcja ``choices`` jest tablicą,
gdzie kluczem jest wartość elementu, a wartością jest etykieta elementu::

    $builder->add('gender', 'choice', array(
        'choices' => array('m' => 'Male', 'f' => 'Female')
    ));

choice_list
~~~~~~~~~~~

**typ**: ``Symfony\Component\Form\Extension\Core\ChoiceList\ChoiceListInterface``

Jest to jeden ze sposobów określenia opcji, które uzywa się dla tego pola.
Opcja ``choice_list`` musi być instancją interfejsu ``ChoiceListInterface``.
Dla bardziej zaawansowanych przypadków, w celu dostarczenie pól typu *choice*,
można utworzyć własną klasa implementującą ten interfejs.

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

Opcje odziedziczone
-------------------

Opcje odziedziczone z typu :doc:`field</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. include:: /reference/forms/types/options/by_reference.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc


