.. index::
    single: SimpleCms; Getting Started
    single: CmfSimpleCmsBundle

A Simple CMS
============

Concept
-------

In most CMS use cases the most basic needs are :doc:`routing <routing>`,
that allows you to associate URLs with your :doc:`content <static_content>`
as well as a way to :doc:`structuring content <structuring_content>`, so
users can browse the content in a menu.

In the CMF these features are provided by the
:doc:`../bundles/routing/introduction`, the :doc:`../bundles/content` and the
:doc:`../bundles/menu`. These bundles complement each other but are
independent, allowing you to choose which ones you want to use, extend or
ignore. However, in some cases, you might just want a simple implementation
that gathers all those capabilities into a ready-to-go package. That's exactly
what the :doc:`../bundles/simple_cms` provides.

SimpleCmsBundle
---------------

The SimpleCmsBundle is implemented on top of most of the other Symfony CMF
Bundles, combining them into a functional CMS. It is a simple solution, but you
will find it very useful when you start implementing your own CMS using
Symfony CMF.

.. tip::

    The SimpleCmsBundle is just an example how you can do it. Whether you
    decide to extend or replace it, it's up to you, but in both cases, the
    SimpleCmsBundle is a good place to start developing your first CMS.

Page Document
~~~~~~~~~~~~~

The SimpleCmsBundle provides a ``Page`` document which provides all roles in
one class:

* It has properties for title and text body;
* It extends the ``Route`` class from the CMF RoutingBundle to work with the
  CMF router component;
* It implements the ``RouteAwareInterface`` to allow the CMF router to
  generate the URL to a page;
* It implements ``NodeInterface``, which means it can be used by
  CMF MenuBundle to generate a menu structure;
* It implements the ``PublishWorkflowInterface`` to be used with the
  :ref:`publish workflow checker <bundle-core-publish_workflow>`.

Here's how that works in practice:

* The routing component receives a request that it matches to a ``Route``
  instance loaded from persistent storage. That ``Route`` is a ``Page``
  instance;
* The route enhancer asks the page for its content and will receive ``$this``,
  putting the page into the request attributes;
* Other route enhancers determine the controller to use with this class
  and optionally the template to use (either a specific template stored with
  the page or one configured in the application configuration for the
  SimpleCmsBundle);
* The controller renders the page using the template, usually generating
  HTML content.
* The template might also render the menu, which will load all Pages and
  build a menu with them.

This three-in-one approach is the key concept behind the bundle.

Configuring the Content Class
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SimpleCmsBundle will use
``Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page`` as the content
class (which extends ``Symfony\Cmf\Bundle\SimpleCmsBundle\Model\Page``). You
can explicitly specify your content class using the configuration parameters:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_simple_cms:
            # defaults to Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page (see above)
            document_class: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <!-- document-class: defaults to Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page (see above) -->
            <config xmlns="http://cmf.symfony.com/schema/dic/simplecms"
                document-class="null"
            />
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_simple_cms', array(
            // defaults to Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page or MultilangPage (see above)
            'document_class' => null,
        ));

Learn More
----------

For more information on the SimpleCmsBundle, please refer to:

* ":doc:`../bundles/simple_cms`" - for more details about the SimpleCmsBundle.
