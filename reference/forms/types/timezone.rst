.. index::
   single: Forms; Fields; timezone

timezone Typ Pola
=================

Typ ``timezone`` jest podzbiorem ``ChoiceType`` który umożliwia użytkownikowi
wybrać jeden z wszystkich dostępnych stref czasowych.

"Wartość" dla każdej strefy czasowej jest pełną nazwą strefy czasowej, np. ``America/Chicago``
lub ``Europe/Istanbul``.

W przeciwieństwie do typu ``choice``, nie musisz definiować opcji ``choices`` lub ``choice_list``,
ponieważ pole automatycznie wypełni je dużą listą lokalizacji.
*Możesz* zdefiniować te opcje ręcznie, w takim przypadku musisz użyć bezpośrednio opcji ``choice``.

+------------------+----------------------------------------------------------------------------+
| Renderowane jako | może być generowane na kilka sposobów (zobacz forms-reference-choice-tags) |
+------------------+----------------------------------------------------------------------------+
| Odziedziczone    | - `multiple`_                                                              |
| opcje            | - `expanded`_                                                              |
|                  | - `preferred_choices`_                                                     |
|                  | - `empty_value`_                                                           |
|                  | - `error_bubbling`_                                                        |
|                  | - `required`_                                                              |
|                  | - `label`_                                                                 |
|                  | - `read_only`_                                                             |
+------------------+----------------------------------------------------------------------------+
| Rodzic           | :doc:`choice</reference/forms/types/choice>`                               |
+------------------+----------------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TimezoneType`     |
+------------------+----------------------------------------------------------------------------+

Odziedziczone opcje
-------------------

Opcje te są odziedziczone z typu :doc:`choice</reference/forms/types/choice>`:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

Opcje te są odziedziczone z typu :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
