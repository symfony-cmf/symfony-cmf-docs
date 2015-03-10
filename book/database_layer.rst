.. index::
    single: Database Layer
    single: PHPCR-ODM

The Database Layer: PHPCR-ODM
=============================

The Symfony CMF is storage layer agnostic, meaning that it can work with many
storage layers. By default, the Symfony CMF works with the `Doctrine PHPCR-ODM`_.
In this chapter, you will learn how to work with the Doctrine PHPCR-ODM.

.. tip::

    Read more about choosing the correct storage layer in
    :doc:`../cookbook/database/choosing_storage_layer`

.. note::

    This chapter assumes you are using a Symfony setup with PHPCR-ODM already
    set up, like the :doc:`CMF Standard Edition <installation>`  or the
    :doc:`CMF sandbox <../cookbook/editions/cmf_sandbox>`. See
    :doc:`../bundles/phpcr_odm/introduction` for how to set up PHPCR-ODM in
    your applications.

PHPCR: A Tree Structure
-----------------------

The Doctrine PHPCR-ODM is a doctrine object-mapper on top of the
`PHP Content Repository`_ (PHPCR), which is a PHP adaption of the
`JSR-283 specification`_. The most important feature of PHPCR is the tree
structure to store the data. All data is stored in items of a tree, called
nodes. You can think of this like a file system, that makes it perfect to use
in a CMS.

On top of the tree structure, PHPCR also adds features like searching,
versioning and access control.

Doctrine PHPCR-ODM has the same API as the other Doctrine libraries, like the
`Doctrine ORM`_. The Doctrine PHPCR-ODM adds another great feature to PHPCR:
Multilanguage support.

.. sidebar:: PHPCR Implementations

    In order to let the Doctrine PHPCR-ODM communicate with the PHPCR, a PHPCR
    implementation is needed. See
    ":doc:`../cookbook/database/choosing_phpcr_implementation`" for an overview
    of the available implementations.

A Simple Example: A Task
------------------------

The easiest way to get started with the PHPCR-ODM is to see it in action. In
this section, you are going to create a ``Task`` object and learn how to
persist it.

Creating a Document Class
~~~~~~~~~~~~~~~~~~~~~~~~~

Without thinking about Doctrine or PHPCR-ODM, you can create a ``Task`` object
in PHP::

    // src/Acme/TaskBundle/Document/Task.php
    namespace Acme\TaskBundle\Document;

    class Task
    {
        protected $description;

        protected $done = false;
    }

This class - often called a "document" in PHPCR-ODM, meaning *a basic class
that holds data* - is simple and helps fulfill the business requirement of
needing tasks in your application. This class can't be persisted to
Doctrine PHPCR-ODM yet - it's just a simple PHP class.

.. note::

    A Document is analogous to the term ``Entity`` employed by the Doctrine
    ORM.  You must add this object to the ``Document`` sub-namespace of you
    bundle, in order register the mapping data automatically.

Add Mapping Information
~~~~~~~~~~~~~~~~~~~~~~~

Doctrine allows you to work with PHPCR in a much more interesting way than
just fetching data back and forth as an array. Instead, Doctrine allows you to
persist entire objects to PHPCR and fetch entire *objects* out of PHPCR.
This works by mapping a PHP class and its properties to the PHPCR tree.

For Doctrine to be able to do this, you just have to create "metadata", or
configuration that tells Doctrine exactly how the ``Task`` document and its
properties should be *mapped* to PHPCR. This metadata can be specified in a
number of different formats including YAML, XML or directly inside the ``Task``
class via annotations:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/TaskBundle/Document/Task.php
        namespace Acme\TaskBundle\Document;

        use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;

        /**
         * @PHPCR\Document()
         */
        class Task
        {
            /**
             * @PHPCR\Id()
             */
            protected $id;

            /**
             * @PHPCR\String()
             */
            protected $description;

            /**
             * @PHPCR\Boolean()
             */
            protected $done = false;

            /**
             * @PHPCR\ParentDocument()
             */
            protected $parentDocument;
        }

    .. code-block:: yaml

        # src/Acme/TaskBundle/Resources/config/doctrine/Task.phpcr.yml
        Acme\TaskBundle\Document\Task:
            id: id

            fields:
                description: string
                done: boolean

            parent_document: parentDocument

    .. code-block:: xml

        <!-- src/Acme/TaskBundle/Resources/config/doctrine/Task.phpcr.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <doctrine-mapping
            xmlns="http://doctrine-project.org/schemas/phpcr-odm/phpcr-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://doctrine-project.org/schemas/phpcr-odm/phpcr-mapping
            https://github.com/doctrine/phpcr-odm/raw/master/doctrine-phpcr-odm-mapping.xsd"
            >

            <document name="Acme\TaskBundle\Document\Task">

                <id name="id" />

                <field name="description" type="string" />
                <field name="done" type="boolean" />

                <parent-document name="parentDocument" />
            </document>

        </doctrine-mapping>

After this, you have to create getters and setters for the properties.

.. note::

    This Document uses the parent document and a node name to determine its
    position in the tree. Because there isn't any name set, it is generated
    automatically. If you want to use a specific node name, such as a
    sluggified version of the title, you need to add a property mapped as
    ``Nodename``.

    A Document must have an id property. This represents the full path (parent
    path + name) of the Document. This will be set by Doctrine by default and
    it is not recommend to use the id to determine the location of a Document.

    For more information about identifier generation strategies, refer to the
    `doctrine documentation`_

.. tip::

    You may want to implement ``Doctrine\ODM\PHPCR\HierarchyInterface``
    which makes it for example possible to leverage the
    :ref:`Sonata Admin Child Extension <bundle-core-child-admin-extension>`.

.. seealso::

    You can also check out Doctrine's `Basic Mapping Documentation`_ for all
    details about mapping information. If you use annotations, you'll need to
    prepend all annotations with ``@PHPCR\``, which is the name of the imported
    namespace (e.g. ``@PHPCR\Document(..)``), this is not shown in Doctrine's
    documentation. You'll also need to include the
    ``use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;`` statement to
    import the PHPCR annotations prefix.

Persisting Documents to PHPCR
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Now that you have a mapped ``Task`` document, complete with getter and setter
methods, you're ready to persist data to PHPCR. From inside a controller,
this is pretty easy, add the following method to the ``DefaultController`` of the
AcmeTaskBundle::

    // src/Acme/TaskBundle/Controller/DefaultController.php

    // ...
    use Acme\TaskBundle\Document\Task;
    use Symfony\Component\HttpFoundation\Response;

    // ...
    public function createAction()
    {
        $documentManager = $this->get('doctrine_phpcr')->getManager();

        $rootTask = $documentManager->find(null, '/tasks');

        $task = new Task();
        $task->setDescription('Finish CMF project');
        $task->setParentDocument($rootTask);

        $documentManager->persist($task);

        $documentManager->flush();

        return new Response('Created task "'.$task->getDescription().'"');
    }

Take a look at the previous example in more detail:

* **line 10** This line fetches Doctrine's *document manager* object, which is
  responsible for handling the process of persisting and fetching objects to
  and from PHPCR.
* **line 12** This line fetches the root document for the tasks, as each
  Document needs to have a parent. To create this root document, you can
  configure a :ref:`Repository Initializer <phpcr-odm-repository-initializers>`,
  which will be executed when running ``doctrine:phpcr:repository:init``.
* **lines 14-16** In this section, you instantiate and work with the ``$task``
  object like any other, normal PHP object.
* **line 18** The ``persist()`` method tells Doctrine to "manage" the
  ``$task`` object. This does not actually cause a query to be made to PHPCR
  (yet).
* **line 20** When the ``flush()`` method is called, Doctrine looks through
  all of the objects that it is managing to see if they need to be persisted to
  PHPCR. In this example, the ``$task`` object has not been persisted yet, so
  the document manager makes a query to PHPCR, which adds a new document.

When creating or updating objects, the workflow is always the same. In the
next section, you'll see how Doctrine is smart enough to update documents if
they already exist in PHPCR.

Fetching Objects from PHPCR
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Fetching an object back out of PHPCR is even easier. For example, suppose
you've configured a route to display a specific task by name::

    public function showAction($name)
    {
        $repository = $this->get('doctrine_phpcr')->getRepository('AcmeTaskBundle:Task');
        $task = $repository->find('/tasks/'.$name);

        if (!$task) {
            throw $this->createNotFoundException('No task found with name '.$name);
        }

        return new Response('['.($task->isDone() ? 'x' : ' ').'] '.$task->getDescription());
    }

To retrieve objects from the document repository using both the ``find`` and
``findMany`` methods and all helper methods of a class-specific repository. In
PHPCR, it's often unkown for developers which node has the data for a specific
document, in that case you should use the document manager to find the nodes
(for instance, when you want to get the root document). In example above, we
know they are ``Task`` documents and so we can use the repository.

The repository contains all sorts of helpful methods::

    // query by the id (full path)
    $task = $repository->find($id);

    // query for one task matching be name and done
    $task = $repository->findOneBy(array('name' => 'foo', 'done' => false));

    // query for all tasks matching the name, ordered by done
    $tasks = $repository->findBy(
        array('name' => 'foo'),
        array('done' => 'ASC')
    );

.. tip::

    If you use the repository class, you can also create a custom repository
    for a specific document. This helps with "Seperation of Concern" when using more
    complex queries. This is similar to how it's done in Doctrine ORM, for
    more information read "`Custom Repository Classes`_" in the core
    documentation.

.. tip::

    You can also query objects by using the Query Builder provided by
    Doctrine PHPCR-ODM. For more information, read
    `the QueryBuilder documentation`_.

Updating an Object
~~~~~~~~~~~~~~~~~~

Once you've fetched an object from Doctrine, updating it is easy. Suppose you
have a route that maps a task ID to an update action in a controller::

    public function updateAction($name)
    {
        $documentManager = $this->get('doctrine_phpcr')->getManager();
        $repository = $documentManager->getRepository('AcmeTaskBundle:Task');
        $task = $repository->find('/tasks/'.$name);

        if (!$task) {
            throw $this->createNotFoundException('No task found for name '.$name);
        }

        if (!$task->isDone()) {
            $task->setDone(true);
        }

        $documentManager->flush();

        return new Response('[x] '.$task->getDescription());
    }

Updating an object involves just three steps:

#. fetching the object from Doctrine;
#. modifying the object;
#. calling ``flush()`` on the document manager

Notice that calling ``$documentManger->persist($task)`` isn't necessary.
Recall that this method simply tells Doctrine to manage or "watch" the
``$task`` object. In this case, since you fetched the ``$task`` object from
Doctrine, it's already managed.

Deleting an Object
~~~~~~~~~~~~~~~~~~

Deleting an object is very similar, but requires a call to the ``remove()``
method of the document manager after you fetched the document from PHPCR::

    $documentManager->remove($task);
    $documentManager->flush();

As you might expect, the ``remove()`` method notifies Doctrine that you'd like
to remove the given document from PHPCR. The actual delete operation
however, is not actually executed until the ``flush()`` method is called.

Summary
-------

With Doctrine, you can focus on your objects and how they're useful in your
application and worry about database persistence second. This is because
Doctrine allows you to use any PHP object to hold your data and relies on
mapping metadata information to map an object's data to a particular database
table.

And even though Doctrine revolves around a simple concept, it's incredibly
powerful, allowing you to `create complex queries`_ and
:doc:`subscribe to events <../bundles/phpcr_odm/events>` that allow you to
take different actions as objects go through their persistence lifecycle.

.. _`Doctrine PHPCR-ODM`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/index.html
.. _`PHP Content Repository`: http://phpcr.github.io/
.. _`JSR-283 specification`: http://jcp.org/en/jsr/detail?id=283
.. _`Doctrine ORM`: http://symfony.com/doc/current/book/doctrine.html
.. _`doctrine documentation`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/basic-mapping.html#basicmapping-identifier-generation-strategies
.. _`Basic Mapping Documentation`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/annotations-reference.html
.. _`the QueryBuilder documentation`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/query-builder.html
.. _`create complex queries`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/query-builder.html
.. _`Custom Repository Classes`: http://symfony.com/doc/current/book/doctrine.html#custom-repository-classes
