Bundle Standards
================

Perhaps unlike many other community bundles, each individual CMF bundle is
part of a larger project, the CMF. As such, stable bundles should adhere to a
core set of standards and goals in addition to the
`suggested bundle best practices`_.

All CMF bundles MUST meet the requirements set out in the following list in
order for them to be classified as stable:

* `General Bundle Standards`_;
* `Meta: README, CHANGELOG, etc`_;
* `Persistence`_;
* `Configuration, Files and Formats`_;
* `Base and Standard Implementations`_;
* `Standard CMF Features`_;
* `Testing Component Integration`_.

The rest of this document will explain each of the above requirements in
detail.

General Bundle Standards
------------------------

Class File Naming
~~~~~~~~~~~~~~~~~

Composite class names SHOULD place the subject first:

+-------------------------+-------------------------+
| Bad                     | Good                    |
+=========================+=========================+
| FoobarMenuNode          | MenuNodeFoobar          |
+-------------------------+-------------------------+
| AllFeaturesSimpleBlock  | SimpleBlockAllFeatures  |
+-------------------------+-------------------------+

Configuration File Naming
~~~~~~~~~~~~~~~~~~~~~~~~~

Configuration files for services SHOULD copy structure of the namespace up to
the level of abstraction required. Each element of a namespace MAY also be
compressed (e.g. event-listener => event).

A suffix may be added to allow different variations of the same configuration
file.

Examples:

* ``event-imagine-phpcr.xml`` and ``event-imagine-orm.xml`` for an event
  subscriber with namespace
  ``[BundleName]\EventListener\ImagineCacheInvalidatorSubscriber``
* ``doctrine-phpcr.xml`` for all classes in the ``[BundleName]\Doctrine\Phpcr``
   namespace.
* ``admin-imagine.xml`` for classes in the ``[BundleName]\Admin\Imagine``
  namespace.

.. _contributing-bundles-interface_naming:

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
"Read/Write" interface MUST incorporate the methods required to fulfill the
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

Refer to the `service naming conventions`_ in the Symfony documentation.

Meta: README, CHANGELOG, etc
----------------------------

Bundles MUST have the following meta files:

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

All CMF bundles:

* MUST support PHPCR-ODM for persistence;
* MAY support other persistence layers like Doctrine ORM;
* MUST follow the following structure to enable future or
  current support of other persistence systems:
* MUST create implementation specific models in addition to
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

.. _contrib_bundles_baseandstandardimplementations:

Base and Standard Implementations
---------------------------------

The CMF offers various features which add functionality beyond the basic
use case of some classes. Examples of these features include multi-language
and publish workflow support, but the potential list of features is unbounded.

Bundles should offer ready-to-use **and** fully integrated implementations in
addition to enabling the user to use only what they need.

To facilitate this, when applicable, there should be **two implementations**,
the *base* implementation and the *standard* implementation.

* **base implementation**: This class should be suffixed with **Base**, e.g.
  ``MenuNodeBase`` and it MUST be an implementation with an absolute minimum
  of logic needed for it to work, it SHOULD NOT have external dependencies;
* **standard implementation**: This class has no additional prefix/suffix, e.g.
  ``MenuNode``. This implementation MUST implement the standard CMF feature
  set. This class MAY have external dependencies.

The user can then extend the **base** implementation, adding any functionality
they want, using the **standard** implementation as a reference.

Standard CMF Features
---------------------

CMF Bundles MUST (where applicable) implement the following features:

* PublishWorkflow;
* Translatable support.

Configuration, Files and Formats
--------------------------------

Core configuration files MUST be in **XML**, this includes:

* Routing;
* Service definitions;
* Doctrine mappings;
* Translations (XLIFF format).

In other cases XML should be preferred over other configuration formats where
there is a choice.

Bundles MUST adhere to the following directory and file name schema
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

.. _`README template on wiki`: https://github.com/symfony-cmf/symfony-cmf/wiki/README-Template
.. _`CHANGELOG template on wiki`: https://github.com/symfony-cmf/symfony-cmf/wiki/Change-log-format
.. _`suggested bundle best practices`: https://symfony.com/doc/current/cookbook/bundles/best_practices.html
.. _`Mapping Model Classes`: https://symfony.com/doc/current/doctrine/mapping_model_classes.html
.. _`DI alias of the bundle`: https://symfony.com/doc/current/bundles/extension.html#creating-an-extension-class
.. _`XML in the configuration class`: https://symfony.com/doc/current/components/config/definition.html#normalization
.. _`XML schema`: https://en.wikipedia.org/wiki/.xsd
.. _`XLIFF format`: https://symfony.com/doc/current/translation.html#basic-translation
.. _`CONTRIBUTING file from CoreBundle`: https://github.com/symfony-cmf/core-bundle/blob/master/CONTRIBUTING.md
.. _`LICENSE template on wiki`: https://github.com/symfony-cmf/symfony-cmf/wiki/LICENSE-Template
.. _`service naming conventions`: https://symfony.com/doc/current/contributing/code/standards.html#service-naming-conventions
