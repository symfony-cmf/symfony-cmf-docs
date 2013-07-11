Bundles
=======

Perhaps unlike many other community bundles, each individual CMF bundle is
part of a larger project, the CMF. As such, stable bundles should adhere to a
core set of standards and goals in addition to the `suggested bundle best
practices`_. 

All CMF bundles MUST meet the requirements set out in the following table in
order for them to be classed as stable:

+----------------------------------+----+
| Feature                          | OK |
+==================================+====+
| Meta: README, CHANGELOG, etc     |    |
+----------------------------------+----+
| Persistance                      |    |
+----------------------------------+----+
| Configuration, files and formats |    |
+----------------------------------+----+
| Testing component integration    |    |
+----------------------------------+----+

The rest of this document will explain each of the above requirements in
detail.

Meta: README, CHANGELOG, etc.
-----------------------------

Bundles MUST have the following metafiles:

.. code-block:: text

    ./README.md
    ./CHANGELOG.md
    ./CONTRIBUTING.md

See the following templates:

* **README**: `README template on wiki`_;
* **CHANGELOG**: `CHANGELOG template on wiki`_;
* **CONTRIBUTING**: `CONTRIBUTING document from CoreBundle`_ (this document is
  the same for all bundles).

Persistance
-----------

* All CMF bundles MUST support PHPCR-ODM;
* A bundle MAY support other persistence layers like Doctrine ORM;
* All CMF bundles MUST follow the following structure to enable future or
  current support of other persistence systems:

.. code-block:: text

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

See the `Mapping Model Classes`_ chapter of the Symfony cookbook for more
information.

Configuration, Files and Formats
--------------------------------

All core configuration files MUST be in **XML**, this includes:

* Routing;
* Service definitions;
* Doctrine mappings.

Bundles MUST adhere to the following directory and filename schema
as applicable:

.. code-block:: text

    ./Resources/
        ./config/
            ./routing
                ./my_service.xml
            ./admin.xml                # all sonata-admin stuff
            ./validation.xml           # all validation
            ./my-related-services.xml  # semanticlly named file for specific services

All bundles MUST define a ``Configuration`` class

.. code-block:: text

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
.. _`CONTRIBUTING document from CoreBundle`: https://github.com/symfony-cmf/CoreBundle/blob/master/CONTRIBUTING.md
.. _`Mapping Model Classes`: http://symfony.com/doc/master/cookbook/doctrine/mapping_model_classes.html

