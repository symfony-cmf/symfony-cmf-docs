Bundles
=======

Perhaps unlike many other community bundles, each individual CMF bundle is
part of a larger project, the CMF. As such, stable bundles should adhere to a
core set of standards and goals in addition to the
`suggested bundle best practices`_.

All CMF bundles MUST meet the requirements set out in the following list in
order for them to be classified as stable:

* `General Bundle Standards`_;
* `Meta: README, CHANGELOG, etc`_;
* `Persistence`_;
* `Configuration, files and formats`_;
* `Testing Component Integration`_.

The rest of this document will explain each of the above requirements in
detail.

General Bundle Standards
------------------------

File Naming
~~~~~~~~~~~

Composite filenames (and by extension class names) SHOULD place the subject
first:

+-------------------------+-------------------------+
| Bad                     | Good                    |
+=========================+=========================+
| ``phpcr-admin.xml``     | ``admin-phpcr.xml``     |
+-------------------------+-------------------------+
| FoobarMenuNode          | MenuNodeFoobar          |
+-------------------------+-------------------------+
| AllFeaturesSimpleBlock  | SimpleBlockAllFeatures  |
+-------------------------+-------------------------+

Interface Naming
~~~~~~~~~~~~~~~~

Interfaces which exist to provide *getters* MUST be suffixed with
"ReadInterface".

Interfaces which exist to provide *setters* MUST be suffixed with
"WriteInterface".

"Read/Write" Interfaces, which provide **both** getters and setters, MUST not
have an additional suffix and MUST extend the "Read" and "Write" interfaces IF
either exists.

If either or both "Read" and "Write" interfaces do not exist, then the
"Read/Write" interface MUST incorporate the methods required to fulfil the
"Read/Write" contract::

    <?php

    interface FoobarInterface extends FoobarReadInterface, FoobarWriteInterface
    {
    }

    // or

    interface FoobarInterface extends FoobarReadInterface
    {
        public function setFoobar($foobar);
    }

    // or

    interface FoobarInterface
    {
        public function getFoobar();

        public function setFoobar($foobar);
    }

Dependency Container Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Refer the the `service naming conventions`_ in the symfony documentation.

Meta: README, CHANGELOG, etc
----------------------------

Bundles MUST have the following metafiles:

.. code-block:: text

    ./README.md
    ./CHANGELOG.md
    ./CONTRIBUTING.md
    ./Resources/meta/LICENSE

See the following templates:

* **README**: `README template on wiki`_;
* **CHANGELOG**: `CHANGELOG template on wiki`_;
* **CONTRIBUTING**: `CONTRIBUTING file from CoreBundle`_ (this should be
  copied verbatim);
* **LICENSE**: `LICENSE template on wiki`_ (this should be
  copied verbatim).

Persistence
-----------

* All CMF bundles MUST support PHPCR-ODM;
* A bundle MAY support other persistence layers like Doctrine ORM;
* All CMF bundles MUST follow the following structure to enable future or
  current support of other persistence systems:
* All CMF bundles MUST create implementation specific models in addition to
  those in ``/Model`` (even if they are empty). See ``Blog.php`` and ``Post.php``
  below.

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

See the `Mapping Model Classes`_ chapter of the Symfony Cookbook for more
information.

Configuration, Files and Formats
--------------------------------

Core configuration files MUST be in **XML**, this includes:

* Routing;
* Service definitions;
* Doctrine mappings;
* Translations (XLIFF format).

In other cases XML should be preferred over other configuration formats where
there is a choice.

Bundles MUST adhere to the following directory and filename schema
as applicable:

.. code-block:: text

    ./Resources/
        ./config/
            ./schema/
                ./bundlename-1.0.xsd
            ./routing
                ./my_service.xml
            ./admin.xml                # all sonata-admin stuff
            ./validation.xml           # all validation
            ./my-related-services.xml  # semantically named file for specific services

Bundles MUST define a ``Configuration`` class:

.. code-block:: text

    ./DependencyInjection
        ./Configuration.php
        ./MyBundleExtension.php

Bundles SHOULD provide an `XML schema`_ for their configuration, as provided by
``Configuration::getXsdValidationBasePath``.

Bundles MUST use their own XML namespace, The XML namespace is
``http://cmf.symfony.com/schema/dic/bundle_name`` with ``bundle_name`` being the
`DI alias of the bundle`_.

Bundles MUST support `XML in the configuration class`_.

Testing Component Integration
-----------------------------

All bundles MUST implement the CMF Testing component.

The :doc:`testing component documentation <../components/testing>` includes
instructions on how the component should be integrated.

.. _`README template on wiki`: https://github.com/symfony-cmf/symfony-cmf/wiki/README
.. _`CHANGELOG template on wiki`: https://github.com/symfony-cmf/symfony-cmf/wiki/Change-log-format
.. _`suggested bundle best practices`: http://symfony.com/doc/current/cookbook/bundles/best_practices.html
.. _`Mapping Model Classes`: http://symfony.com/doc/master/cookbook/doctrine/mapping_model_classes.html
.. _`DI alias of the bundle`: http://symfony.com/doc/current/cookbook/bundles/extension.html#creating-an-extension-class
.. _`XML in the configuration class`: http://symfony.com/doc/current/components/config/definition.html#normalization
.. _`XML schema`: https://en.wikipedia.org/wiki/.xsd
.. _`XLIFF format`: http://symfony.com/doc/current/book/translation.html#basic-translation
.. _`CONTRIBUTING file from CoreBundle`: https://github.com/symfony-cmf/CoreBundle/blob/master/CONTRIBUTING.md
.. _`LICENSE template on wiki`: https://github.com/symfony-cmf/symfony-cmf/wiki/LICENSE-Template
.. _`service naming conventions`: http://symfony.com/doc/current/contributing/code/standards.html#service-naming-conventions
