.. index::
    single: Redirect URLs with a trailing slash

Redirect URLs with a trailing slash
===================================

The goal of this tutorial is to demonstrate how to redirect URLs with
trailing slash to the same url without trailing slash
(for example ``/en/blog/`` to ``/en/blog``).

.. note::

    For the moment, the :doc:`RoutingBundle <../bundles/routing/introduction>`
    can't achieve this automatically.

You have to create a controller which will match any URL with a trailing
slash, remove the trailing slash (keeping query parameters if any) and
redirect to new URL with a 301 response status code::

    // src/Acme/DemoBundle/Controller/RedirectingController.php
    namespace Acme\DemoBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Request;

    class RedirectingController extends Controller
    {
        public function removeTrailingSlashAction(Request $request)
        {
            $pathInfo = $request->getPathInfo();
            $requestUri = $request->getRequestUri();

            $url = str_replace($pathInfo, rtrim($pathInfo, ' /'), $requestUri);

            return $this->redirect($url, 301);
        }
    }

And after that, register this controller to be executed whenever a url
with a trailing slash is requested:

.. configuration-block::

    .. code-block:: yaml

        remove_trailing_slash:
            path: /{url}
            defaults: { _controller: AcmeDemoBundle:Redirecting:removeTrailingSlash }
            requirements:
                url: .*/$
                _method: GET


    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing">
            <route id="remove_trailing_slash" path="/{url}">
                <default key="_controller">AcmeDemoBundle:Redirecting:removeTrailingSlash</default>
                <requirement key="url">.*/$</requirement>
                <requirement key="_method">GET</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('remove_trailing_slash', new Route('/{url}', array(
            '_controller' => 'AcmeDemoBundle:Redirecting:removeTrailingSlash',
        ), array(
            'url' => '.*/$',
            '_method' => 'GET',
        )));
