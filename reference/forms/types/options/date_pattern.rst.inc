pattern
~~~~~~~

**typ**: ``string``

Ta opcja ma znaczenie tylko, gdy `widget`_ jest ustawiony na ``choice``.
Domyślny wzorzeć bazuje na opcji `format` i próbuje dopasować się do wzorca
na podstawie znaków ``M``, ``d`` i ``y``. Jeśli nie zostanie znaleziony, to domyślnie
jest to ciąg znaków ``{{ year }}-{{ month }}-{{ day }}``.
Tokeny dla tej opcji obejmują:

  * ``{{ year }}``: Zastąpiony przez widget ``year``
  * ``{{ month }}``: Zastąpiony przez widget ``month``
  * ``{{ day }}``: Zastąpiony przez widget ``day``