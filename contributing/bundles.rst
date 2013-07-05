Bundles
=======

Perhaps unlike many other bundles, each individual CMF bundle is part of a
larger project, the CMF. As such, stable bundles should adhere to a core set
of standards and goals in addition to the `suggested bundle best practices`_.
We aim to provide:

* A base standard of quality to be established;
* Easier maintainance;
* Less barriers for developers who want to contibute outside of their
  speciality.
* <more>

All CMF bundles should be adhere to the following checklist. The checklist is
in markdown format and can be copied to the bundles ``README.md`` file.

.. code-block:: markdown

    | Feature                          | OK |
    | --                               | -- |
    | Meta: README, CHANGELOG, etc     |    |
    | Persistance                      |    |
    | Configuration, files and formats |    |
    | Testing component integration    |    |

Meta: README, CHANGELOG, etc.
-----------------------------

Bundles MUST have the following metafiles:

.. code-block:: bash

    ./README.md
    ./CHANMGELOG.md
    ./CONTRIBUTING.md

See the following templates:

* **README**: `README template on wiki`_;
* **CHANGELOG**: `CHANGELOG template on wiki`_;
* **CONTRIBUTING**: See existing bundles.

Persistance
-----------

* All CMF bundles MUST support PHPCR-ODM;
* All CMF bundles MUST follow the following structure to enable future or
  current support of other persistance systems:

.. code-block:: bash

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

Configuration, Files and Formats
--------------------------------

All core configuration files MUST be in **XML**, this includes:

* Routing;
* Service definitions;
* Doctrine mappings.

Bundles MUST adhere to the following directory and filenaming structure
as applicable:

.. code-block:: bash

    ./Resources/
        ./config/
            ./routing
                ./my_service.xml
            ./admin.xml                # all sonata-admin stuff
            ./validation.xml           # all validation
            ./my-related-services.xml  # semanticlly named file for specific services

All bundles MUST define a `Configuration` class

.. code-block:: bash

    ./DependencyInjection
        ./Configuration.php
        ./MyBundleExtension.php

Standard Integration of the Testing Component
---------------------------------------------

All bundles MUST implement the CMF Testing component.

The :doc:`testing component documentation <../components/testing>` includes
instructions on how the component should be integrated.

.. _`README template on wiki`: https://github.com/symfony-cmf/symfony-cmf/wiki/README-format-proposal
.. _`CHANGELOG template on wiki`: https://github.com/symfony-cmf/symfony-cmf/wiki/Change-log-format
.. _`suggested bundle best practices`: http://symfony.com/doc/current/cookbook/bundles/best_practices.html
