.. index::
   single: Forms; Fields; text

text Typ Pola
=============

Pole tekstowe reprezentujące podstawowe pole typu text.

+------------------+--------------------------------------------------------------------+
| Renderowane jako | ``input`` ``text`` pole                                            |
+------------------+--------------------------------------------------------------------+
| Odziedziczone    | - `max_length`_                                                    |
| opcje            | - `required`_                                                      |
|                  | - `label`_                                                         |
|                  | - `trim`_                                                          |
|                  | - `read_only`_                                                     |
|                  | - `error_bubbling`_                                                |
+------------------+--------------------------------------------------------------------+
| Rodzic           | :doc:`field</reference/forms/types/field>`                         |
+------------------+--------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TextType` |
+------------------+--------------------------------------------------------------------+


Odziedziczone opcje
-------------------

Opcje te są odziedziczone z typu :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
