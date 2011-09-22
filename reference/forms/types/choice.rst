.. index::
   single: Forms; Fields; choice

choice Typ Pola
===============

Wielofunkcyjne pole używane do umożliwienia użytkownikowi "wybrania" jednej lub kilku opcji.
Może być wyrenderowany jako pole ``select``, przyciski radio, lub checkboxy.

Aby użyć tego pola, musisz zdefiniować *jedno* z opcji ``choice_list`` lub ``choices``.

+------------------+-----------------------------------------------------------------------------+
| Rednerowany jako | może być generowany na kilka sposobów (zobacz poniżej)                      |
+------------------+-----------------------------------------------------------------------------+
| Opcje            | - `choices`_                                                                |
|                  | - `choice_list`_                                                            |
|                  | - `multiple`_                                                               |
|                  | - `expanded`_                                                               |
|                  | - `preferred_choices`_                                                      |
|                  | - `empty_value`_                                                            |
+------------------+-----------------------------------------------------------------------------+
| Odziedziczone    | - `required`_                                                               |
| opcje            | - `label`_                                                                  |
|                  | - `read_only`_                                                              |
|                  | - `error_bubbling`_                                                         |
+------------------+-----------------------------------------------------------------------------+
| Rodzic           | :doc:`form</reference/forms/types/form>` (if expanded), ``field`` otherwise |
+------------------+-----------------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\ChoiceType`        |
+------------------+-----------------------------------------------------------------------------+

Przykład Użycia
---------------

Najprostszym sposobem na użycie tego pola jest zdefiniowane możliwych opcji wyboru poprzez
opcję ``choices``. Klucz tablicy staje się wartością która zostanie zapisana w Twoim obiekcie
(np. ``m``), a wartość jest tym co zobaczy użytkownik w formularzu (np. ``Male``).

.. code-block:: php

    $builder->add('gender', 'choice', array(
        'choices'   => array('m' => 'Male', 'f' => 'Female'),
        'required'  => false,
    ));

Ustawiając opcję ``multiple`` na true, zezwolisz użytkownikowi na zaznaczenie kilku opcji. 
Widget zostanie wygenerowany jako lista wielokrotnego wyboru ``select`` lub też lista
checkboxów, w zależności od wyboru opcji ``expanded``:

.. code-block:: php

    $builder->add('availability', 'choice', array(
        'choices'   => array(
            'morning'   => 'Morning',
            'afternoon' => 'Afternoon',
            'evening'   => 'Evening',
        ),
        'multiple'  => true,
    ));

Możesz także użyć opcji ``choice_list``, która to pobiera obiekt który może
definiować możliwe opcje wyboru w widgecie.

.. _forms-reference-choice-tags:

.. include:: /reference/forms/types/options/select_how_rendered.rst.inc

Opcje Pola
----------

choices
~~~~~~~

**typ**: ``array`` **domyślnie**: ``array()``

Jest to najbardziej podstawowa możliwość definiowania opcji wyboru
które powinny być użyte przez te pole. Opcja ``choices`` jest tablicą,
gdzie kluczem tablicy jest wartość opcji, a wartość tablicy jest etykietą
opcji::

    $builder->add('gender', 'choice', array(
        'choices' => array('m' => 'Male', 'f' => 'Female')
    ));

choice_list
~~~~~~~~~~~

**typ**: ``Symfony\Component\Form\Extension\Core\ChoiceList\ChoiceListInterface``

Jest to jeden z sposobów definiowania opcji użytych dla tego pola.
Opcja ``choice_list`` musi być instancją interfejsu ``ChoiceListInterface``.
Dla bardziej zaawansowanych przypadków, można stworzyć zwyczajną klasę
implementującą ten interfejs do zasilenia opcji wyboru.

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

Odziedziczone opcje
-------------------

Opcje te są odziedziczone z typu :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
