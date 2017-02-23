.. index::
    single: TreeBrowser; Bundles
    single: TreeBrowserBundle

TreeBrowserBundle
=================

    The TreeBrowserBundle provides a tree view to visualize the backend and
    make it easy to manage the backend. It uses the ResourceRestBundle to get
    the nodes.

Installation
------------

You can install this bundle `with composer`_ using the
``symfony-cmf/tree-browser-bundle`` package on `Packagist`_.

The CmfTreeBrowserBundle, ResourceRestBundle and ResourceBundle must be
registered in the ``AppKernel``::

    // app/appKernel.php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Symfony\Cmf\Bundle\ResourceBundle\CmfResourceBundle(),
            new Symfony\Cmf\Bundle\ResourceRestBundle\CmfResourceRestBundle(),
            new Symfony\Cmf\Bundle\TreeBrowserBundle\CmfTreeBrowserBundle(),
        );

        // ...

        return $bundles;
    }

Routing
~~~~~~~

The ResourceRestBundle provides a REST interface using Symfony routes. In order
to make this work, you have to import the routes in your routing configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        cmf_resource_rest:
            resource: "@CmfResourceRest/Resources/config/routing.xml"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@CmfResourceRestBundle/Resources/config/routing.xml" />

        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import(
            '@CmfResourceRestBundle/Resources/config/routing.xml'
        ));

        return $collection;

Usage
-----

The TreeBrowserBundle provides a two jQuery plugins, ready for you to use:

* :doc:`$(elem).cmfTree() <tree>`
* :doc:`$(elem).cmfContextMenu() <context_menu>`

The bundle also ships a ``TreeSelectType`` form type. Using this form type,
users can select nodes from the tree instead of typing the path in a text
field. Read more about this form type in :doc:`it's own section <forms>`.

Read On
-------

* :doc:`forms`
* :doc:`tree`
* :doc:`configuration`
* :doc:`context_menu`

.. _`Packagist`: https://packagist.org/packages/symfony-cmf/tree-browser-bundle
.. _`with composer`: http://getcomposer.org
.. _`admin_tree.js`: https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/master/Resources/views/Tree/tree.html.twig
.. _`select_tree.js`: https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/master/Resources/views/Form/form_admin_fields.html.twig
.. _`PhpcrOdmTree`: https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/master/Tree/PhpcrOdmTree.php
.. _`jsTree`: http://www.jstree.com/
.. _SonataDoctrinePHPCRAdminBundle: http://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/index.html
