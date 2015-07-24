.. index::
    single: Menu; Bundles
    single: MenuBundle

MenuBundle
==========

The CmfMenuBundle extends the `KnpMenuBundle`_ and adds the following
features:

* Render menus stored in the persistence layer;
* Generate menu node URLs from linked Content or Route.

Note that only the Doctrine PHPCR-ODM persistence layer is supported in the
1.0 release.

.. caution::

    Make sure you understand how the `KnpMenuBundle`_ works when you want to
    customize the CmfMenuBundle. The CMF menu bundle is just adding
    functionality on top of the Knp menu bundle and this documentation is
    focused on the additional functionality.

Installation
------------

You can install this bundle `with composer`_ using the
`symfony-cmf/menu-bundle`_ package.

As the bundle is using the `KnpMenuBundle`_, you need to instantiate that
bundles in addition to the CmfMenuBundle::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Knp\Bundle\MenuBundle\KnpMenuBundle(),
                new Symfony\Cmf\Bundle\MenuBundle\CmfMenuBundle(),
            );

            // ...
        }

        // ...
    }

Creating a Simple Persistent Menu
---------------------------------

A menu created using the KnpMenuBundle is made up of a hierarchy of class
instances implementing ``NodeInterface``. This is also true of a menu created
using MenuBundle documents.

It is recommended that the root document of the menu tree is a ``Menu``
document, all descendant documents should be ``MenuNode`` instances.

The root document should be a child of the document specified in the configuration
by the parameter ``persistence.phpcr.menu_basepath``, which defaults to ``/cms/menu``. Note
that if this document does not exist it must be created.

The example below creates a new menu with two items, "Home" and "Contact",
each of which specifies a URI::

    use Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\MenuNode;
    use Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\Menu;
    use PHPCR\Util\NodeHelper;

    // this node should be created automatically, see note below this example
    $menuParent = $dm->find(null, '/cms/menu');

    $menu = new Menu();
    $menu->setName('main-menu');
    $menu->setLabel('Main Menu');
    $menu->setParentDocument($menuParent);

    $manager->persist($menu);

    $home = new MenuNode();
    $home->setName('home');
    $home->setLabel('Home');
    $home->setParentDocument($menu);
    $home->setUri('http://www.example.com/home');

    $manager->persist($home);

    $contact = new MenuNode();
    $contact->setName('contact');
    $contact->setLabel('Contact');
    $contact->setParentDocument($menu);
    $contact->setUri('http://www.example.com/contact');

    $manager->persist($contact);

    $manager->flush();

.. note::

    When the bundle is registered, it will create the ``/cms/menu`` path
    when executing ``doctrine:phpcr:repository:init``. For more information,
    see :ref:`Repository Initializers <phpcr-odm-repository-initializers>`

Rendering Menus
---------------

You render menus in the same way you would with the `KnpMenuBundle`_. The name
of the menu will correspond to the name of the root document in your menu
tree:

.. configuration-block::

    .. code-block:: jinja

        {{ knp_menu_render('main-menu') }}

    .. code-block:: php

        echo $view['knp_menu']->render('main-menu');

Here the ``main-menu`` document from the previous
example is specified. This will render an unordered list as follows:

.. code-block:: html

    <ul>
        <li class="first">
          <a href="http://www.example.com/home">Home</a>
        </li>
        <li class="last">
          <a href="http://www.example.com/contact">Contact</a>
        </li>
    </ul>

.. tip::

    Sometimes, the menu is not located within the ``persistence.phpcr.menu_basepath``.
    In this case, you can use an absolute path (starting with a forward slash) to render
    the menu:

    .. configuration-block::

        .. code-block:: jinja

            {{ knp_menu_render('/cms/some/path/my-menu') }}

        .. code-block:: php

            echo $view['knp_menu']->render('/cms/some/path/my-menu');

.. tip::

    When using the :doc:`BlockBundle <../block/introduction>`, you can also
    use the ``MenuBlock``. Read more about that in the
    :ref:`BlockBundle documentation <bundles-block-menu>`

.. note::

     It is the ``PhpcrMenuProvider`` class which allows us to specify a
     PHPCR-ODM document as a menu. For more information see the
     :doc:`menu provider documentation <menu_provider>`.

.. caution::

    If you want to render the menu from Twig, make sure you have not disabled
    Twig in the ``knp_menu`` configuration section.

For more information see the `rendering menus`_ section of the KnpMenuBundle documentation.

.. _`KnpMenu`: https://github.com/knplabs/KnpMenu
.. _`KnpMenuBundle`: https://github.com/knplabs/KnpMenuBundle
.. _`with composer`: http://getcomposer.org
.. _`rendering menus`: https://github.com/KnpLabs/KnpMenuBundle/blob/master/Resources/doc/index.md#rendering-menus
.. _`symfony-cmf/menu-bundle`: https://packagist.org/packages/symfony-cmf/menu-bundle
