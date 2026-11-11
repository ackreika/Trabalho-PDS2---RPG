#include "doctest.h"
#include "Movimento.hpp"

TEST_CASE("Testando a classe Movimento") {
    SUBCASE("Movimento com usos ilimitados") {
        Movimento golpe("Derivada Fatal", 40, -1);

        CHECK(golpe.getNome() == "Derivada Fatal");
        CHECK(golpe.getDano() == 40);
        CHECK(golpe.podeUsar() == true);

        golpe.usar();
        CHECK(golpe.podeUsar() == true); // Continua podendo usar pois é ilimitado (-1)

        golpe.aumentarPoder(10);
        CHECK(golpe.getDano() == 50);
    }

    SUBCASE("Movimento com usos limitados e reset") {
        Movimento especial("Integral Tripla", 80, 2);

        CHECK(especial.getUsosRestantes() == 2);
        CHECK(especial.podeUsar() == true);

        especial.usar();
        CHECK(especial.getUsosRestantes() == 1);

        especial.usar();
        CHECK(especial.getUsosRestantes() == 0);
        CHECK(especial.podeUsar() == false); // Acabou a munição!

        especial.resetarUsos();
        CHECK(especial.getUsosRestantes() == 2);
        CHECK(especial.podeUsar() == true);
    }

    SUBCASE("Catálogo de movimentos e desbloqueio gradual por vitórias") {
        CHECK(Movimento::getQuantidadeCatalogo() == 12);

        // Os 3 primeiros movimentos começam desbloqueados (0 vitórias necessárias)
        Movimento m0 = Movimento::criarMovimentoPorId(0);
        CHECK(m0.getVitoriasNecessarias() == 0);
        CHECK(m0.estaDesbloqueado(0) == true);

        // Série de Fourier (ID 3) requer 1 vitória
        Movimento m3 = Movimento::criarMovimentoPorId(3);
        CHECK(m3.getVitoriasNecessarias() == 1);
        CHECK(m3.estaDesbloqueado(0) == false);
        CHECK(m3.estaDesbloqueado(1) == true);
        CHECK(!m3.getResumoEfeito().empty());
    }
}
