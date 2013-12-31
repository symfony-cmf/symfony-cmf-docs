Controllers and Templates
-------------------------

Go to the URL http://localhost:8000/page/home in your browser - this should be
your page, but it says that it cannot find a controller. In other words it has
found the *page referencing route* for your page but Symfony does not know what
to do with it.

You can map a default controller for all instances of ``Page``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            dynamic:
                # ...
                controllers_by_class:
                    Acme\BasicCmsBundle\Document\Page: Acme\BasicCmsBundle\Controller\DefaultController::pageAction

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic generic-controller="cmf_content.controller:indexAction">
                    <!-- ... -->
                    <controllers-by-class
                        class="Acme\BasicCmsBundle\Document\Page"
                    >
                        Acme\BasicCmsBundle\Controller\DefaultController::pageAction
                    </controllers-by-class>
                </dynamic>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                // ...
                'controllers_by_class' => array(
                    'Acme\BasicCmsBundle\Document\Page' => 'Acme\BasicCmsBundle\Controller\DefaultController::pageAction',
                ),
            ),
        ));

This will cause requests to be forwarded to this controller when the route
which matches the incoming request is provided by the dynamic router **and**
the content document that that route references is of class
``Acme\BasicCmsBundle\Document\Page``.

Now create the action in the default controller - you can pass the ``Page``
object and all the ``Posts`` to the view::

    // src/Acme/BasicCmsBundle/Controller/DefaultController.php

    // ...
    class DefaultController extends Controller
    {
        // ...

        /**
         * @Template()
         */
        public function pageAction($contentDocument)
        {
            $dm = $this->get('doctrine_phpcr')->getManager();
            $posts = $dm->getRepository('Acme\BasicCmsBundle\Document\Post')->findAll();

            return array(
                'page'  => $contentDocument,
                'posts' => $posts,
            );
        }
    }

The ``Page`` object is passed automatically as ``$contentDocument``.

Add a corresponding twig template (note that this works because you use the
``@Template`` annotation):

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/BasicCmsBundle/Resources/views/Default/page.html.twig #}
        <h1>{{ page.title }}</h1>
        <p>{{ page.content|raw }}</p>
        <h2>Our Blog Posts</h2>
        <ul>
            {% for post in posts %}
                <li><a href="{{ path(post) }}">{{ post.title }}</a></li>
            {% endfor %}
        </ul>

    .. code-block:: html+php

        <!-- src/Acme/BasicCmsBundle/Resources/views/Default/page.html.twig -->
        <h1><?php echo $page->getTitle() ?></h1>
        <p><?php echo $page->getContent() ?></p>
        <h2>Our Blog Posts</h2>
        <ul>
            <?php foreach($posts as $post) : ?>
                <li>
                    <a href="<?php echo $view['router']->generate($post) ?>">
                        <?php echo $post->getTitle() ?>
                    </a>
                </li>
            <?php endforeach ?>
        </ul>

Now have another look at: http://localhost:8000/page/home

Notice what is happening with the post object and the ``path`` function  - you
pass the ``Post`` object and the ``path`` function will pass the object to the
router and because it implements the ``RouteReferrersReadInterface`` the
``DynamicRouter`` will be able to generate the URL for the post.

Click on a ``Post`` and you will have the same error that you had before when
viewing the page at ``/home`` and you can resolve it in the same way.
