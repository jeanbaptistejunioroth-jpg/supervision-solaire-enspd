// Mode vitrine : actif uniquement si SHOWCASE_MODE=1 (défini seulement sur Vercel).
// En local (ESP32 ou fichiers ouverts directement), ce fichier n'est jamais exécuté.
import { next } from '@vercel/functions';

const ROBOTS = 'noindex, nofollow';

// Pages publiques autorisées en mode vitrine (accueil, présentation, mentions légales, démos)
const PUBLIC_PAGES = new Set([
  '/', '/index.html',
  '/mentions-legales', '/mentions-legales.html',
  '/interface/fonctionnement%20normal.html',
  '/interface/panne.html',
  '/interface/interface%20prototype.html',
]);
const STATIC_ASSET = /\.(css|js|png|jpe?g|gif|svg|webp|ico|woff2?|ttf|txt)$/i;
// Endpoints de l'ESP32 + toute route API
const API = /^\/(api|data|cmd|maint)(\/|$)/i;

export default function middleware(request) {
  if (process.env.SHOWCASE_MODE !== '1') return next();

  const url = new URL(request.url);
  const path = url.pathname;

  if (API.test(path)) {
    return new Response(
      JSON.stringify({ error: 'Service indisponible : site en mode vitrine.' }),
      { status: 503, headers: { 'content-type': 'application/json; charset=utf-8', 'X-Robots-Tag': ROBOTS, 'Retry-After': '86400' } }
    );
  }

  if (PUBLIC_PAGES.has(path) || path === '/robots.txt' || STATIC_ASSET.test(path)) {
    return next({ headers: { 'X-Robots-Tag': ROBOTS } });
  }

  // Connexion, inscription, espace client, paiement, admin, etc. -> accueil
  return new Response(null, { status: 307, headers: { Location: '/', 'X-Robots-Tag': ROBOTS } });
}

export const config = { matcher: '/:path*' };
