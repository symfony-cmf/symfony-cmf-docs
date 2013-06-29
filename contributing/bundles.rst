Bundles
=======

All bundles should be standardized as much as possible.

Standardization facilitates easy maintenance and interoperability on the numerous CMF. 

All CMF bundles (and components where applicable) should adhere to the following:

- Agnostic persistence models;
- Standard configuration file formats;
- Standard integration of the Testing component.

Agnostic Persistence Models
---------------------------

.. code-block::

    ./Model
         ./Blog.php
         ./Post.php
    ./Doctrine
        ./Phpcr
            ./Blog.php
            ./Post.php
            ./PostRepository.php
            ./PostListener.php
        ./Orm
    ./Resources/
        ./config
            ./doctrine-phpcr
                ./Blog.phpcr.xml

Standard Configuration Formats
------------------------------

XML should be the configuration language for the following:

- DI container configuration
- Doctrine persistence mappings

Standard Integration of the Testing Component
---------------------------------------------

All bundles should implement the CMF Testing component.

The :doc:`testing component documentation <../components/testing>` includes
instructions on how the component should be integrated.

Approach to complexity
----------------------

CMF Bundles are primarily intended to be foundations that end-user
applications can build upon. For this reason we should always supply the most
basic implementation possible in addition to the most advanced implementation
possible.

For example:

- **MenuNode**: A simple menu node;
- **MenuNode**: Advanced menu node with translations link types, etc.
