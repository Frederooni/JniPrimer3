/*
 * Copyright (C) 2016 Michigan State University Board of Trustees
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package edu.msu.cme.rdp.primerdesign.utils;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * @author gunturus
 */

public class Primer3Wrapper {
    private double _mv = 50; // mN conc of monovalent cations
    private double _dv = 1.5; // mN conc of divalent cations
    private double _dntp = 0.6; // mN ntp concentration
    private double _dna = 50.0; // nM DNA concentration
    private double _temp_c = 37; //
    private int _max_loop = 30; // smaller is faster
    private int _tm_method = 1; //
    private int _salt_method = 1; //

    private static Object lock = new Object();

    public Primer3Wrapper() {
    }

    public Primer3Wrapper(double sodiumMv, double magnesDv) {
        this._mv = sodiumMv;
        this._dv = magnesDv;
    }

    private native double calcTm(String seq, double d, double mv, double dv,
                                 double n, int tm_method, int salt_method);

    private native double calcThermo(String seq1, String seq2, int maxLoop,
                                     double mv, double dv, double dntp, double dna_conc, double temp,
                                     int temponly, int dimer, int aligntype);

    public double calcTemp(String seq) {
        synchronized (lock) {
            return calcTm(seq, _dna, _mv, _dv, _dntp, _tm_method, _salt_method);
        }
    }

    /** Call Primer3's <code>thal</code> function. */
    public double thal(String seq1, String seq2, String type) {
        synchronized (lock) {
            if (type.equals("HAIRPIN")) {
                return calcThermo(seq1, seq2, _max_loop, _mv, _dv, _dntp, _dna, _temp_c, 1, 0, 4);
            } else if (type.equals("END1")) {
                return calcThermo(seq1, seq2, _max_loop, _mv, _dv, _dntp, _dna, _temp_c, 1, 0, 2);
            } else if (type.equals("END2")) {
                return calcThermo(seq1, seq2, _max_loop, _mv, _dv, _dntp, _dna, _temp_c, 1, 0, 3);
            } else if (type.equals("ANY")) {
                return calcThermo(seq1, seq2, _max_loop, _mv, _dv, _dntp, _dna, _temp_c, 1, 0, 1);
            } else {
                throw new RuntimeException("Unknown alignment type " + type);
            }
        }
    }

    public double calcHairpinTm(String seq) {
        synchronized (lock) {
            return thal(seq, seq, "HAIRPIN");
        }
    }

    public double calcHomodimerTm(String seq) {
        return thal(seq, seq, "ANY");
    }

    public double calcHetrodimerTm(String seq1, String seq2) {
        return thal(seq1, seq2, "ANY");
    }

    public void setMv(int mv) {
        this._mv = mv;
    }

    public void setDv(double dv) {
        this._dv = dv;
    }

    public void setDntp(double dntp) {
        this._dntp = dntp;
    }

    public void setDna(double dna) {
        this._dna = dna;
    }

    public void setTempC(double tempC) {
        this._temp_c = tempC;
    }

    public void setMaxLoop(int maxloop) {
        this._max_loop = maxloop;
    }

    public void setTmMeth(int tmMeth) {
        this._tm_method = tmMeth;
    }

    public void setSaltMeth(int saltMeth) {
        this._salt_method = saltMeth;
    }

    public double getMv() {
        return _mv;
    }

    public double getDv() {
        return _dv;
    }

    public double getDntp() {
        return _dntp;
    }

    public double getDna() {
        return _dna;
    }

    public double getTempC() {
        return _temp_c;
    }

    public int getMaxLoop() {
        return _max_loop;
    }

    public int getTmMeth() {
        return _tm_method;
    }

    public int getSaltMeth() {
        return _salt_method;
    }

    public static void main(String[] args) throws Exception {
        Primer3Wrapper primer3 = new Primer3Wrapper(50.0, 1.5);
        if (args.length == 1) {
            double tm = primer3.calcTemp(args[0]);
            System.out.printf("%s %f\n", args[0], tm);
        } else if (args.length == 2) {
            double tm = primer3.calcHetrodimerTm(args[0], args[1]);
            System.out.printf("%s %s %f\n", args[0], args[1], tm);
        } else {
            System.err.print("java -jar JavaPrimer3.jar oligo1 [oligo2]\n");
            System.exit(1);
        }
    }

    // Based on https://stackoverflow.com/a/1611367
    static {
        String name = "/libJavaPrimer3.so";
        try {
            InputStream in = Primer3Wrapper.class.getResourceAsStream(name);
            // long ms = System.currentTimeMillis;
            File file = new File(System.getProperty("java.io.tmpdir") + "/" + name);
            OutputStream out = new FileOutputStream(file);
            byte[] buffer = new byte[1024];
            int c;
            while ((c = in.read(buffer)) >= 0) {
                out.write(buffer, 0, c);
            }
            in.close();
            out.close();
            name = file.toString();
            System.load(file.toString());
        } catch (Exception e) {
            throw new RuntimeException("Failed to load " + name, e);
        }
    }
}
