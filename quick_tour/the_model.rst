.. index::
    single: The Model; Quick Tour

The Model
=========

You decided to continue reading 10 more minutes about the Symfony CMF? That's
great news! In this part, you will learn more about the default database layer
of the CMF.

.. note::

    Again, this chapter is talking about the PHPCR storage layer. But the CMF
    is written in a storage agnostic way, meaning it is not tied to specific
    storage system.

Getting Familiar with PHPCR
---------------------------

PHPCR_ stores all data into one big tree structure. You can compare this to a
filesystem where each file and directory contains data. This means that all
data stored with PHPCR has a relationship with at least one other data: its
parent. The inverse relation also exists, you can also get the children of a
data element.

Let's take a look at the dump of the tree of the CMF Sandbox you
downloaded in the previous chapter. Go to your directory and execute the
following command:

.. code-block:: bash

    $ php bin/console doctrine:phpcr:node:dump

The result will be the PHPCR tree:

.. code-block:: text

    ROOT:
        cms:
            menu:
                main:
                    admin-item:
                    projects-item:
                        cmf-item:
                    company-item:
                        team-item:
                ...
            content:
                home:
                    phpcr_locale:en:
                    phpcr_locale:fr:
                    phpcr_locale:de:
                    seoMetadata:
                    additionalInfoBlock:
                        child1:
                ...
            routes:
                en:
                    company:
                        team:
                        more:
                    about:
                ...

Each data is called a *node* in PHPCR. Everything is attached under the ROOT
node (created by PHPCR itself).

Each node has properties, which contain the data. The content, title and label
you set for your page are saved in such properties for the ``home``
node. You can view these properties by adding the ``--props`` switch to the
dump command:

.. code-block:: bash

    $ php bin/console doctrine:phpcr:node:dump --props /cms/content/home

.. note::

    Previously, the PHPCR tree was compared with a Filesystem. While this
    gives you a good image of what happens, it's not the truth. You can
    better compare it to an XML file, where each node is an element and its
    properties are attributes.

Doctrine PHPCR-ODM
------------------

The Symfony CMF uses the `Doctrine PHPCR-ODM`_ to interact with PHPCR.
Doctrine allows a user to create objects (called *documents*) which are
directly persisted into and retrieved from the PHPCR tree. This is similar to
the Doctrine ORM provided by default in the Symfony Standard Edition, but for
PHPCR instead of SQL databases.

Creating a Page with code
-------------------------

Now you know a little bit more about PHPCR and you know the tool to interact
with it, you can start using it yourself. In the previous chapter, you edited
a page by using a yaml file which was parsed by the fixture loader of the
sandbox. This time, you'll create a page by doing it yourself.

First, you have to create a new DataFixture to add your new page. You do this
by creating a new class in the AppBundle::

    // src/AppBundle/DataFixtures/PHPCR/LoadQuickTourData.php
    namespace AppBundle\DataFixtures\PHPCR;

    use Doctrine\Common\Persistence\ObjectManager;
    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;

    class LoadPageData implements FixtureInterface, OrderedFixtureInterface
    {
        public function getOrder()
        {
            // refers to the order in which the class' load function is called
            // (lower return values are called first)
            return 100;
        }

        public function load(ObjectManager $documentManager)
        {
        }
    }

The ``$documentManager`` is the object which will persist the document to
PHPCR. But first, you have to create a new Page document::

    use Doctrine\ODM\PHPCR\DocumentManager;
    use Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page;

    // ...
    public function load(ObjectManager $documentManager)
    {
        if (!$documentManager instanceof DocumentManager) {
            $class = get_class($documentManager);
            throw new \RuntimeException("Fixture requires a PHPCR ODM DocumentManager instance, instance of '$class' given.");
        }

        $page = new Page(); // create a new Page object (document)
        $page->setName('quick-tour'); // the name of the node

        vno sandbox übernehmen

        $page->setLabel('Another new Page');
        $page->setTitle('Another new Page');
        $page->setBody('I have added this page myself!');
    }

Each document needs a parent. In this case, the parent should just be the root
node. To do this, we first retrieve the root document from PHPCR and then set
it as its parent::

    public function load(ObjectManager $documentManager)
    {
        // ...
        // get root document (/cms/simple)
        $simpleCmsRoot = $documentManager->find(null, '/cms/simple');

        $page->setParentDocument($simpleCmsRoot); // set the parent to the root
    }

And at last, we have to tell the Document Manager to persist our Page
document using the Doctrine API::

    public function load(ObjectManager $documentManager)
    {
        // ...
        $documentManager->persist($page); // add the Page in the queue
        $documentManager->flush(); // add the Page to PHPCR
    }

Now you need to execute the ``doctrine:phpcr:fixtures:load`` command again.
When dumping the nodes again, your new page should turn up under /cms/content.

To actually see this page in the browser, we need a route::

    public function load(ObjectManager $documentManager)
    {
        // ...
        $route = new Route();
        $routeRoot = $documentManager->find(null, '/cms/routes/en');
        $route->setPosition($routeRoot, 'quick-tour');
        $route->setContent($page);
        $documentManager->persist($route);
    }

And we add a menu entry to link to this page, and flush the document manager
at the end::

    public function load(ObjectManager $documentManager)
    {
        $menu = new MenuNode();
        $menu->setName('new_page');
        $menu->setLabel('Quick Tour');
        $menu->setContent($page);
        $menuMain = $documentManager->find(null, '/cms/menu/main');
        $menu->setParentDocument($menuMain);
        $documentManager->persist($menu);

        $documentManager->flush();
    }

Re-run the fixtures loading command and then refresh the web site. Your new
page will be added, with a menu entry at the bottom of the menu!

.. image:: ../_images/quick_tour/the-model-new-page.png

.. seealso::

    See ":doc:`../book/database_layer`" if you want to know more about using
    PHPCR in a Symfony application.

Final Thoughts
--------------

PHPCR is a powerful way to store your pages in a CMS. But, if you're not
comfortable with it, you can always
:doc:`switch to another storage layer <../cookbook/database/choosing_storage_layer>`.

When looking back at these 20 minutes, you should have learned how to work
with a new storage layer and you have added 2 new pages. Do you see how easy
the CMF works when making your application editable? It provides most of the
things you previously had to do yourself.

But you have now only seen a small bit of the CMF, there is much more to learn
about and many other bundles are waiting for you. Before you can do all this,
you should meet the backbone of the CMF: The routing system. You can read
about that in :doc:`the next chapter <the_router>`. Ready for another 10
minutes?

.. _PHPCR: http://phpcr.github.io/
.. _`Doctrine PHPCR-ODM`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/
