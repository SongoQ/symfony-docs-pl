.. index::
   single: Forms; Fields; email

email Typ Pola
==============

Pole ``email`` jest polem tekstowym, które jest renderowane używając HTML5 jako
tag ``<input type="email" />``.

+---------------------+---------------------------------------------------------------------+
| Renderowane jako    | ``input`` ``email`` pole (tekstowe)                                 |
+---------------------+---------------------------------------------------------------------+
| Odziedziczone       | - `max_length`_                                                     |
| opcje               | - `required`_                                                       |
|                     | - `label`_                                                          |
|                     | - `trim`_                                                           |
|                     | - `read_only`_                                                      |
|                     | - `error_bubbling`_                                                 |
+---------------------+---------------------------------------------------------------------+
| Rodzic              | :doc:`field</reference/forms/types/field>`                          |
+---------------------+---------------------------------------------------------------------+
| Klasa               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\EmailType` |
+---------------------+---------------------------------------------------------------------+

Odziedziczone opcje
-------------------

Opcje te są odziedziczone z typu :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
