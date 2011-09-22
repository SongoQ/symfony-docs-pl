.. index::
   single: Forms; Fields; country

country Typ Pola
=======================

Typ ``country`` jest podzbiorem ``ChoiceType`` który wyświetla kraje świata.
Dodatkowym bonusem, jest to że nazwy krajów wyświetlane są w języku użytkownika.

"Wartością" każdego kraju jest dwu literowy kod kraju.

.. note::

   Lokalizacja Twojego użytkownika jest pobierana poprzez `Locale::getDefault()`_

W przeciwieństwie do typu ``choice``, nie musisz definiować opcji ``choices`` lub
``choice_list``, ponieważ pole automatycznie wypełni je wszystkimi krajami świata.
*Możesz* zdefiniować te opcje ręcznie, w takim przypadku musisz użyć bezpośrednio
opcji ``choice``.

+------------------+--------------------------------------------------------------------------------------------+
| Renderowane jako | może być generowane na kilka sposobów (zobacz :ref:`forms-reference-choice-tags`)          |
+------------------+--------------------------------------------------------------------------------------------+
| Odziedziczone    | - `multiple`_                                                                              |
| opcje            | - `expanded`_                                                                              |
|                  | - `preferred_choices`_                                                                     |
|                  | - `empty_value`_                                                                           |
|                  | - `error_bubbling`_                                                                        |
|                  | - `required`_                                                                              |
|                  | - `label`_                                                                                 |
|                  | - `read_only`_                                                                             |
+------------------+--------------------------------------------------------------------------------------------+
| Rodzic           | :doc:`choice</reference/forms/types/choice>`                                               |
+------------------+--------------------------------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CountryType`                      |
+------------------+--------------------------------------------------------------------------------------------+

Odziedziczone opcje
-------------------

Opcje te są odziedziczone z typu :doc:`choice</reference/forms/types/choice>`:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

Opcje te są odziedziczone z typu :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. _`Locale::getDefault()`: http://php.net/manual/en/locale.getdefault.php
